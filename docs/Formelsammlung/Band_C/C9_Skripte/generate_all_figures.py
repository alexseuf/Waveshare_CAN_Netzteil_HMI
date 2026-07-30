"""Erzeugt alle Diagramme des dokumentierten PFC-Drosselbeispiels neu.

Aufruf:
    python generate_all_figures.py
    python generate_all_figures.py --output <Zielverzeichnis>

Das Skript erzeugt SVG und PNG sowie eine CSV-Datei mit berechneten
Kennwerten. Mathematische Grundlagen: docs/Formelsammlung/Band_C/C9.md
"""

from __future__ import annotations

import argparse
import csv
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Circle, Rectangle

from parameter import PARAMS, ChokeParameters

MU0 = 4.0e-7 * np.pi


def validate(p: ChokeParameters) -> None:
    if p.turns <= 0 or p.effective_area_m2 <= 0 or p.effective_length_m <= 0:
        raise ValueError("Windungszahl und magnetische Geometrie müssen positiv sein.")
    if p.current_support_a.ndim != 1 or p.ldiff_support_h.ndim != 1:
        raise ValueError("Ldiff-Stützstellen müssen eindimensional sein.")
    if len(p.current_support_a) != len(p.ldiff_support_h):
        raise ValueError("Strom- und Ldiff-Stützstellen müssen gleich lang sein.")
    if np.any(np.diff(p.current_support_a) <= 0):
        raise ValueError("Stromstützstellen müssen streng monoton steigen.")
    if np.any(p.ldiff_support_h <= 0):
        raise ValueError("Alle Ldiff-Werte müssen positiv sein.")
    l0 = initial_inductance(p)
    if not np.isclose(p.ldiff_support_h[0], l0, rtol=0.08):
        raise ValueError(
            f"Ldiff(0)={p.ldiff_support_h[0]*1e6:.1f} µH ist nicht konsistent "
            f"mit L0={l0*1e6:.1f} µH."
        )


def initial_inductance(p: ChokeParameters) -> float:
    return MU0 * p.mu_r0 * p.turns**2 * p.effective_area_m2 / p.effective_length_m


def core_volume_m3(p: ChokeParameters) -> float:
    return p.effective_area_m2 * p.effective_length_m


def ldiff(i_a: np.ndarray | float, p: ChokeParameters) -> np.ndarray:
    return np.interp(
        np.abs(np.asarray(i_a, dtype=float)),
        p.current_support_a,
        p.ldiff_support_h,
        left=p.ldiff_support_h[0],
        right=p.ldiff_support_h[-1],
    )


def cumulative_trapezoid(y: np.ndarray, x: np.ndarray) -> np.ndarray:
    result = np.zeros_like(y)
    result[1:] = np.cumsum(0.5 * (y[1:] + y[:-1]) * np.diff(x))
    return result


def magnetic_curves(p: ChokeParameters) -> dict[str, np.ndarray]:
    i = np.linspace(0.0, p.current_plot_max_a, p.current_plot_points)
    l_diff = ldiff(i, p)
    psi = cumulative_trapezoid(l_diff, i)
    l_sec = np.empty_like(i)
    l_sec[0] = l_diff[0]
    l_sec[1:] = psi[1:] / i[1:]
    mu_diff = p.mu_r0 * l_diff / l_diff[0]
    mu_sec = p.mu_r0 * l_sec / l_diff[0]
    h_oe = 4.0 * np.pi * p.turns * i / (p.effective_length_m * 1e3)
    b = np.interp(
        i,
        p.b_current_support_a,
        p.b_support_t,
        left=p.b_support_t[0],
        right=p.b_support_t[-1],
    )
    return {
        "i": i,
        "l_diff": l_diff,
        "psi": psi,
        "l_sec": l_sec,
        "mu_diff": mu_diff,
        "mu_sec": mu_sec,
        "h_oe": h_oe,
        "b": b,
    }


def electrical_period(p: ChokeParameters, i_rms: float) -> dict[str, np.ndarray]:
    dt = 1.0 / (p.switching_frequency_hz * p.points_per_switching_period)
    t = np.arange(0.0, 1.0 / p.mains_frequency_hz, dt)
    omega = 2.0 * np.pi * p.mains_frequency_hz
    u_phase_peak = np.sqrt(2.0) * p.line_voltage_rms_v / np.sqrt(3.0)
    u_phase = u_phase_peak * np.sin(omega * t)
    duty = 0.5 + u_phase / p.dc_link_voltage_v
    i1 = np.sqrt(2.0) * i_rms * np.sin(omega * t)
    l_local = ldiff(i1, p)
    delta_i_pp = (
        p.dc_link_voltage_v
        * duty
        * (1.0 - duty)
        / (l_local * p.switching_frequency_hz)
    )
    phase_sw = np.mod(t * p.switching_frequency_hz, 1.0)
    triangle = 4.0 * np.abs(phase_sw - 0.5) - 1.0
    i_total = i1 + 0.5 * delta_i_pp * triangle
    i_upper = i1 + 0.5 * delta_i_pp
    i_lower = i1 - 0.5 * delta_i_pp
    delta_b_pp = (
        p.dc_link_voltage_v
        * duty
        * (1.0 - duty)
        / (p.turns * p.effective_area_m2 * p.switching_frequency_hz)
    )
    return {
        "t": t,
        "u_phase": u_phase,
        "duty": duty,
        "i1": i1,
        "l_local": l_local,
        "delta_i_pp": delta_i_pp,
        "triangle": triangle,
        "i_total": i_total,
        "i_upper": i_upper,
        "i_lower": i_lower,
        "delta_b_pp": delta_b_pp,
    }


def save_figure(fig: plt.Figure, output: Path, stem: str) -> None:
    output.mkdir(parents=True, exist_ok=True)
    fig.savefig(output / f"{stem}.svg", bbox_inches="tight")
    fig.savefig(output / f"{stem}.png", dpi=220, bbox_inches="tight")
    plt.close(fig)


def plot_mechanical(p: ChokeParameters, output: Path) -> None:
    fig, ax = plt.subplots(figsize=(9, 5))
    ax.set_aspect("equal")
    radius_outer = p.outer_diameter_m * 500
    radius_inner = p.inner_diameter_m * 500
    for k in range(p.core_count):
        x = k * radius_outer * 0.32
        ax.add_patch(Circle((x, 0), radius_outer, fill=False, linewidth=3))
        ax.add_patch(Circle((x, 0), radius_inner, fill=False, linewidth=2))
    ax.plot([-radius_outer, p.core_count * radius_outer * 0.32], [-radius_outer - 8] * 2, linewidth=4)
    ax.text(0, radius_outer + 12, f"{p.core_count} × {p.core_name}", ha="center")
    ax.text(0, -radius_outer - 18, "schematischer, gestapelter Ringkernaufbau", ha="center")
    ax.set_xlim(-radius_outer - 20, p.core_count * radius_outer * 0.32 + radius_outer + 20)
    ax.set_ylim(-radius_outer - 30, radius_outer + 30)
    ax.axis("off")
    ax.set_title("Mechanischer Aufbau")
    save_figure(fig, output, "abbildung_01_mechanischer_aufbau")


def plot_winding(p: ChokeParameters, output: Path) -> None:
    fig, ax = plt.subplots(figsize=(10, 4.5))
    ax.add_patch(Rectangle((0.1, 0.2), 0.8, 0.55, fill=False, linewidth=3))
    for n in range(p.turns_layer_1):
        x = 0.12 + 0.76 * n / max(p.turns_layer_1 - 1, 1)
        ax.plot([x, x], [0.24, 0.45], linewidth=2)
    for n in range(p.turns_layer_2):
        x = 0.16 + 0.68 * n / max(p.turns_layer_2 - 1, 1)
        ax.plot([x, x], [0.50, 0.70], linewidth=2, linestyle="--")
    ax.text(0.5, 0.15, f"Lage 1: {p.turns_layer_1} Wdg", ha="center")
    ax.text(0.5, 0.79, f"Lage 2: {p.turns_layer_2} Wdg", ha="center")
    ax.text(0.5, 0.05, f"Litze {p.strand_count} × {p.strand_diameter_m*1e3:.2f} mm", ha="center")
    ax.set_xlim(0, 1)
    ax.set_ylim(0, 0.9)
    ax.axis("off")
    ax.set_title("Schematischer Wicklungsaufbau")
    save_figure(fig, output, "abbildung_02_wicklungsaufbau")


def plot_magnetic(p: ChokeParameters, output: Path) -> None:
    m = magnetic_curves(p)

    fig, ax = plt.subplots(figsize=(9, 5.5))
    ax.plot(m["h_oe"], m["b"])
    for current, label in zip(
        [np.sqrt(2) * x for x in p.load_currents_rms_a], p.load_labels
    ):
        h = np.interp(current, m["i"], m["h_oe"])
        b = np.interp(current, m["i"], m["b"])
        ax.scatter([h], [b])
        ax.annotate(label, (h, b), xytext=(6, 6), textcoords="offset points")
    ax.set_xlabel("Feldstärke H [Oe]")
    ax.set_ylabel("Flussdichte B [T]")
    ax.set_title("B(H)-Kennlinie")
    ax.grid(True)
    save_figure(fig, output, "abbildung_03_bh_kennlinie")

    fig, ax = plt.subplots(figsize=(9, 5.5))
    ax.plot(m["i"], m["l_diff"] * 1e6, label="Ldiff")
    ax.plot(m["i"], m["l_sec"] * 1e6, label="Lsec")
    ax.axhline(initial_inductance(p) * 1e6, linestyle="--", label="L0")
    ax.set_xlabel("Strom I [A]")
    ax.set_ylabel("Induktivität L [µH]")
    ax.set_title("Differentielle und Sekanteninduktivität")
    ax.grid(True)
    ax.legend()
    save_figure(fig, output, "abbildung_04_induktivitaet")

    fig, ax = plt.subplots(figsize=(9, 5.5))
    ax.plot(m["i"], m["mu_diff"], label="µr,diff")
    ax.plot(m["i"], m["mu_sec"], label="µr,sec")
    ax.set_xlabel("Strom I [A]")
    ax.set_ylabel("Relative Permeabilität µr")
    ax.set_title("Differentielle und Sekantenpermeabilität")
    ax.grid(True)
    ax.legend()
    save_figure(fig, output, "abbildung_05_permeabilitaet")


def copper_resistance(p: ChokeParameters, temperature_c: float) -> float:
    return p.winding_resistance_25_ohm * (
        1.0 + p.copper_temperature_coefficient_per_k * (temperature_c - 25.0)
    )


def plot_losses_and_thermal(p: ChokeParameters, output: Path) -> dict[str, float]:
    currents = np.linspace(0.0, max(p.load_currents_rms_a) * 1.05, 300)
    r25 = copper_resistance(p, 25.0)
    r_hot = copper_resistance(p, p.hot_winding_temperature_c)
    pcu25 = currents**2 * r25 * p.ac_resistance_factor
    pcu_hot = currents**2 * r_hot * p.ac_resistance_factor

    fig, ax = plt.subplots(figsize=(9, 5.5))
    ax.plot(currents, pcu25, label="25 °C")
    ax.plot(currents, pcu_hot, label=f"{p.hot_winding_temperature_c:.0f} °C")
    ax.set_xlabel("Effektivstrom I_rms [A]")
    ax.set_ylabel("Kupferverluste P_Cu [W]")
    ax.set_title("Kupferverluste")
    ax.grid(True)
    ax.legend()
    save_figure(fig, output, "abbildung_06_kupferverluste")

    periods = [electrical_period(p, x) for x in p.load_currents_rms_a]
    volume_cm3 = core_volume_m3(p) * 1e6
    core_means: list[float] = []
    fig, ax = plt.subplots(figsize=(9, 5.5))
    for data, label in zip(periods, p.load_labels):
        bpk = data["delta_b_pp"] / 2.0
        pv_mw_cm3 = (
            p.steinmetz_a
            * np.power(np.maximum(bpk, 0.0), p.steinmetz_b)
            * np.power(p.switching_frequency_hz / 1e3, p.steinmetz_c)
        )
        pcore = pv_mw_cm3 * volume_cm3 / 1e3
        core_means.append(float(np.mean(pcore)))
        ax.plot(data["t"] * 1e3, pcore, label=label)
    ax.set_xlabel("Zeit t [ms]")
    ax.set_ylabel("Kernverlustleistung P_core [W]")
    ax.set_title("Momentane Kernverlustleistung über eine Netzperiode")
    ax.grid(True)
    ax.legend()
    save_figure(fig, output, "abbildung_07_kernverluste")

    total_hot = []
    for i_rms, pcore in zip(p.load_currents_rms_a, core_means):
        total_hot.append(i_rms**2 * r_hot * p.ac_resistance_factor + pcore)
    fig, ax = plt.subplots(figsize=(8, 5.5))
    x = np.arange(len(p.load_labels))
    ax.bar(x, total_hot)
    ax.set_xticks(x, p.load_labels)
    ax.set_ylabel("Gesamtverluste P_ges [W]")
    ax.set_title(f"Gesamtverluste bei {p.hot_winding_temperature_c:.0f} °C")
    ax.grid(True, axis="y")
    save_figure(fig, output, "abbildung_08_gesamtverluste")

    t_overload = np.linspace(0.0, p.overload_duration_s, 300)
    delta_t = total_hot[-1] * t_overload / p.thermal_capacity_j_per_k
    fig, ax = plt.subplots(figsize=(9, 5.5))
    ax.plot(t_overload, delta_t)
    ax.set_xlabel("Zeit t [s]")
    ax.set_ylabel("Adiabatischer Temperaturanstieg ΔT [K]")
    ax.set_title("Temperaturanstieg während der Spitzenlast")
    ax.grid(True)
    save_figure(fig, output, "abbildung_09_temperaturanstieg")

    return {
        "r25_ohm": r25,
        "rhot_ohm": r_hot,
        "pcore_mean_20w": core_means[0],
        "pcore_mean_40w": core_means[1],
        "ptotal_hot_20w": total_hot[0],
        "ptotal_hot_40w": total_hot[1],
        "delta_t_overload_k": float(delta_t[-1]),
    }


def plot_ripple(p: ChokeParameters, output: Path) -> dict[str, float]:
    phi = np.linspace(0.0, 2.0 * np.pi, p.mains_angle_points)
    u_peak = np.sqrt(2.0) * p.line_voltage_rms_v / np.sqrt(3.0)
    duty = 0.5 + u_peak * np.sin(phi) / p.dc_link_voltage_v
    l0 = initial_inductance(p)
    ripple_l0 = p.dc_link_voltage_v * duty * (1 - duty) / (
        l0 * p.switching_frequency_hz
    )

    fig, ax = plt.subplots(figsize=(9, 5.5))
    ax.plot(np.degrees(phi), ripple_l0)
    ax.set_xlabel("Netzwinkel φ [°]")
    ax.set_ylabel("Stromwelligkeit ΔI_pp [A]")
    ax.set_title("Stromwelligkeit bei konstanter Anfangsinduktivität")
    ax.grid(True)
    save_figure(fig, output, "abbildung_10_stromwelligkeit_netzwinkel")

    fig, ax = plt.subplots(figsize=(9, 5.5))
    periods = []
    for i_rms, label in zip(p.load_currents_rms_a, p.load_labels):
        i1 = np.sqrt(2.0) * i_rms * np.sin(phi)
        ripple = p.dc_link_voltage_v * duty * (1 - duty) / (
            ldiff(i1, p) * p.switching_frequency_hz
        )
        ax.plot(np.degrees(phi), ripple, label=label)
        periods.append(electrical_period(p, i_rms))
    ax.plot(np.degrees(phi), ripple_l0, linestyle="--", label="konstantes L0")
    ax.set_xlabel("Netzwinkel φ [°]")
    ax.set_ylabel("Stromwelligkeit ΔI_pp [A]")
    ax.set_title("Arbeitspunktabhängige Stromwelligkeit")
    ax.grid(True)
    ax.legend()
    save_figure(fig, output, "abbildung_11_stromwelligkeit_ldiff_grundwelle")

    fig, ax = plt.subplots(figsize=(9, 5.5))
    delta_b = p.dc_link_voltage_v * duty * (1 - duty) / (
        p.turns * p.effective_area_m2 * p.switching_frequency_hz
    )
    for label in p.load_labels:
        ax.plot(np.degrees(phi), delta_b * 1e3, label=label)
    ax.set_xlabel("Netzwinkel φ [°]")
    ax.set_ylabel("Flussdichtehub ΔB_pp [mT]")
    ax.set_title("PWM-Flussdichtehub")
    ax.grid(True)
    ax.legend()
    save_figure(fig, output, "abbildung_12_flussdichtehub_ldiff_grundwelle")

    fig, ax = plt.subplots(figsize=(9, 5.5))
    i_rms_axis = np.linspace(0.0, max(p.load_currents_rms_a) * 1.1, 400)
    i_peak_axis = np.sqrt(2.0) * i_rms_axis
    b_gw = np.interp(
        i_peak_axis,
        p.b_current_support_a,
        p.b_support_t,
        left=p.b_support_t[0],
        right=p.b_support_t[-1],
    )
    b_total = b_gw + np.max(delta_b) / 2.0
    ax.plot(i_rms_axis, b_gw, label="Grundwelle")
    ax.plot(i_rms_axis, b_total, label="Grundwelle + maximaler PWM-Anteil")
    ax.axhline(p.b_sat_t, linestyle="--", label="B_sat")
    ax.set_xlabel("Grundwellen-Effektivstrom I_rms [A]")
    ax.set_ylabel("Maximale Flussdichte B [T]")
    ax.set_title("Ausnutzung des Kernmaterials")
    ax.grid(True)
    ax.legend()
    save_figure(fig, output, "abbildung_13_ausnutzung_kernmaterial_bmax")

    fig = plt.figure(figsize=(14, 9))
    ax1 = fig.add_axes([0.08, 0.54, 0.88, 0.38])
    for data, label in zip(periods, p.load_labels):
        ax1.plot(data["t"] * 1e3, data["i_total"], linewidth=0.65, label=f"{label}: Gesamtstrom")
        ax1.plot(data["t"] * 1e3, data["i1"], linestyle="--", linewidth=1.2, label=f"{label}: Grundwelle")
        ax1.plot(data["t"] * 1e3, data["i_upper"], linewidth=0.7, alpha=0.7)
        ax1.plot(data["t"] * 1e3, data["i_lower"], linewidth=0.7, alpha=0.7)
    ax1.set_xlim(0, 20)
    ax1.set_xlabel("Zeit t [ms]")
    ax1.set_ylabel("Phasenstrom i(t) [A]")
    ax1.set_title("Berechnete Überlagerung: 50-Hz-Grundwelle und 70-kHz-Dreiecksrippel")
    ax1.grid(True)
    ax1.legend(ncol=2)

    ax2 = fig.add_axes([0.08, 0.10, 0.62, 0.30])
    zoom_start, zoom_end = 4.85e-3, 5.15e-3
    for data, label in zip(periods, p.load_labels):
        mask = (data["t"] >= zoom_start) & (data["t"] <= zoom_end)
        ax2.plot(data["t"][mask] * 1e3, data["i_total"][mask], label=label)
        ax2.plot(data["t"][mask] * 1e3, data["i1"][mask], linestyle="--")
    ax2.set_xlabel("Zeit t [ms]")
    ax2.set_ylabel("Phasenstrom i(t) [A]")
    ax2.set_title("Detail am positiven Grundwellen-Scheitel")
    ax2.grid(True)
    ax2.legend()

    text = []
    stats: dict[str, float] = {}
    for data, label in zip(periods, p.load_labels):
        key = "20" if "20" in label else "40"
        stats[f"ripple_min_{key}_a"] = float(np.min(data["delta_i_pp"]))
        stats[f"ripple_max_{key}_a"] = float(np.max(data["delta_i_pp"]))
        stats[f"current_max_{key}_a"] = float(np.max(data["i_total"]))
        stats[f"current_min_{key}_a"] = float(np.min(data["i_total"]))
        text.extend([
            label,
            f"ΔIpp = {np.min(data['delta_i_pp']):.2f} … {np.max(data['delta_i_pp']):.2f} A",
            f"i_max = {np.max(data['i_total']):.2f} A",
            "",
        ])
    fig.text(0.74, 0.12, "\n".join(text), va="bottom")
    save_figure(fig, output, "abbildung_09_stromverlauf_50Hz_70kHz")

    stats["delta_b_min_t"] = float(np.min(delta_b))
    stats["delta_b_max_t"] = float(np.max(delta_b))
    return stats


def write_csv(output: Path, p: ChokeParameters, stats: dict[str, float]) -> None:
    path = output / "berechnete_kennwerte.csv"
    rows = [
        ("L0", initial_inductance(p), "H"),
        ("Kernvolumen", core_volume_m3(p), "m3"),
    ]
    rows.extend((key, value, "SI") for key, value in sorted(stats.items()))
    with path.open("w", newline="", encoding="utf-8") as file:
        writer = csv.writer(file, delimiter=";")
        writer.writerow(["Kennwert", "Wert", "Einheit"])
        writer.writerows(rows)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--output",
        type=Path,
        default=Path(__file__).resolve().parent / "generated",
        help="Ausgabeverzeichnis für SVG, PNG und CSV",
    )
    args = parser.parse_args()
    p = PARAMS
    validate(p)
    args.output.mkdir(parents=True, exist_ok=True)

    plot_mechanical(p, args.output)
    plot_winding(p, args.output)
    plot_magnetic(p, args.output)
    loss_stats = plot_losses_and_thermal(p, args.output)
    ripple_stats = plot_ripple(p, args.output)
    write_csv(args.output, p, {**loss_stats, **ripple_stats})

    print(f"Alle Diagramme wurden unter {args.output} erzeugt.")


if __name__ == "__main__":
    main()
