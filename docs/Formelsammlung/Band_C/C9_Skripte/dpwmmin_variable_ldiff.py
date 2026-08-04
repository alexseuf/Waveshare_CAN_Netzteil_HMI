#!/usr/bin/env python3
"""DPWMmin-Stromwelligkeit mit stromabhängiger differentieller Induktivität.

Berechnet für Phase A den Verlauf Delta-I_pp über eine 50-Hz-Netzperiode.
Die Minimum-Clamp-SVM wird schaltzustandsbasiert mit einem symmetrischen
Dreiecksträger abgebildet. Pro PWM-Periode wird die tatsächliche
Phasendrosselspannung integriert.

Optional kann eine CSV-Datei mit einer gemessenen oder digitalisierten
Ldiff(I)-Kennlinie verwendet werden. Erwartete Spalten:
    current_a, ldiff_h
oder
    current_a, ldiff_uh

Ohne CSV wird die dokumentierte Tanh-Entwurfsnäherung verwendet:
    B(H) = B_sat * tanh(mu0 * mu_r0 * H / B_sat)
    Ldiff(I) = L0 * sech(alpha * I)^2
"""

from __future__ import annotations

import argparse
import csv
from pathlib import Path
from typing import Callable

import matplotlib.pyplot as plt
import numpy as np

U_LL_RMS = 400.0
U_DC = 750.0
F_GRID = 50.0
F_SW = 70_000.0
L0 = 584e-6
MU_R0 = 26.0
B_SAT = 1.65
N_WDG = 80
L_MAG = 0.127
MU0 = 4.0 * np.pi * 1e-7
PWM_SAMPLES = 240


def load_ldiff_csv(path: Path) -> Callable[[np.ndarray], np.ndarray]:
    """Load an even Ldiff(|I|) characteristic from CSV."""
    currents: list[float] = []
    inductances: list[float] = []
    with path.open(newline="", encoding="utf-8") as handle:
        reader = csv.DictReader(handle)
        if reader.fieldnames is None or "current_a" not in reader.fieldnames:
            raise ValueError("CSV benötigt die Spalte 'current_a'.")
        use_h = "ldiff_h" in reader.fieldnames
        use_uh = "ldiff_uh" in reader.fieldnames
        if not use_h and not use_uh:
            raise ValueError("CSV benötigt 'ldiff_h' oder 'ldiff_uh'.")
        for row in reader:
            current = abs(float(row["current_a"]))
            value = float(row["ldiff_h"] if use_h else row["ldiff_uh"])
            inductance = value if use_h else value * 1e-6
            currents.append(current)
            inductances.append(inductance)
    x = np.asarray(currents)
    y = np.asarray(inductances)
    order = np.argsort(x)
    x = x[order]
    y = y[order]
    if len(x) < 2 or np.any(np.diff(x) <= 0):
        raise ValueError("Stromstützstellen müssen eindeutig und streng monoton sein.")
    if np.any(y <= 0):
        raise ValueError("Alle Induktivitäten müssen positiv sein.")

    def interpolate(current: np.ndarray) -> np.ndarray:
        return np.interp(np.abs(current), x, y, left=y[0], right=y[-1])

    return interpolate


def tanh_ldiff(current: np.ndarray) -> np.ndarray:
    """Differential inductance from the documented tanh magnetization fit."""
    alpha = MU0 * MU_R0 * N_WDG / (L_MAG * B_SAT)
    return L0 / np.cosh(alpha * current) ** 2


def calculate_phase_a_ripple(
    i_rms: float,
    ldiff_function: Callable[[np.ndarray], np.ndarray],
) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
    """Calculate electrical angle, Delta-Ipp and local Ldiff for phase A."""
    t_sw = 1.0 / F_SW
    n_pwm = int(round((1.0 / F_GRID) / t_sw))
    pwm_index = np.arange(n_pwm)
    time_mid = (pwm_index + 0.5) * t_sw
    theta = 2.0 * np.pi * F_GRID * time_mid

    u_phase_peak = np.sqrt(2.0) * U_LL_RMS / np.sqrt(3.0)
    modulation = u_phase_peak / (U_DC / 2.0)
    references = np.vstack((
        modulation * np.sin(theta),
        modulation * np.sin(theta - 2.0 * np.pi / 3.0),
        modulation * np.sin(theta + 2.0 * np.pi / 3.0),
    ))

    # DPWMmin: kleinster Referenzwert wird an den negativen Zwischenkreis geklemmt.
    common_mode = -1.0 - np.min(references, axis=0)
    references_dpwm = references + common_mode
    duty = np.clip((references_dpwm + 1.0) / 2.0, 0.0, 1.0)

    carrier_time = (np.arange(PWM_SAMPLES) + 0.5) / PWM_SAMPLES
    carrier = 1.0 - np.abs(2.0 * carrier_time - 1.0)
    dt = t_sw / PWM_SAMPLES

    fundamental_current = np.sqrt(2.0) * i_rms * np.sin(theta)
    ldiff = ldiff_function(fundamental_current)
    ripple_pp = np.zeros(n_pwm)

    for index in range(n_pwm):
        switching = (duty[:, index, None] >= carrier[None, :]).astype(float)
        phase_voltage = U_DC * (
            switching - np.mean(switching, axis=0, keepdims=True)
        )
        ripple_voltage_a = phase_voltage[0] - np.mean(phase_voltage[0])
        ripple_current_a = np.cumsum(ripple_voltage_a) * dt / ldiff[index]
        ripple_pp[index] = np.ptp(ripple_current_a)

    return np.degrees(theta) % 360.0, ripple_pp, ldiff


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--ldiff-csv",
        type=Path,
        help="Optionale CSV mit current_a und ldiff_h oder ldiff_uh.",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("dpwmmin_delta_ipp_phase_a.svg"),
    )
    args = parser.parse_args()

    ldiff_function = load_ldiff_csv(args.ldiff_csv) if args.ldiff_csv else tanh_ldiff
    operating_points = {
        "Leerlauf": 0.0,
        "20 kW": 20_000.0 / (np.sqrt(3.0) * U_LL_RMS),
        "40 kW": 40_000.0 / (np.sqrt(3.0) * U_LL_RMS),
    }
    results = {
        label: calculate_phase_a_ripple(current_rms, ldiff_function)
        for label, current_rms in operating_points.items()
    }

    figure, axis = plt.subplots(figsize=(11.5, 6.4))
    for label in ("Leerlauf", "20 kW", "40 kW"):
        angle, ripple, _ = results[label]
        axis.plot(angle, ripple, label=label)
    axis.set_xlabel("Elektrischer Netzwinkel θ [°]")
    axis.set_ylabel("Stromwelligkeit Phase A ΔIpp [A]")
    axis.set_title(
        "Minimum-Clamp-SVM (DPWMmin): Stromwelligkeit der Phase A\n"
        "mit stromabhängiger differentieller Induktivität"
    )
    axis.set_xlim(0.0, 360.0)
    axis.set_xticks(np.arange(0.0, 361.0, 30.0))
    axis.grid(True)
    axis.legend()
    figure.tight_layout()

    args.output.parent.mkdir(parents=True, exist_ok=True)
    figure.savefig(args.output, bbox_inches="tight")
    if args.output.suffix.lower() != ".png":
        figure.savefig(args.output.with_suffix(".png"), dpi=180, bbox_inches="tight")

    print("Arbeitspunkt; I_RMS [A]; Ldiff_min [uH]; DeltaIpp_max [A]")
    for label, current_rms in operating_points.items():
        _, ripple, ldiff = results[label]
        print(
            f"{label}; {current_rms:.3f}; "
            f"{np.min(ldiff) * 1e6:.1f}; {np.max(ripple):.3f}"
        )


if __name__ == "__main__":
    main()
