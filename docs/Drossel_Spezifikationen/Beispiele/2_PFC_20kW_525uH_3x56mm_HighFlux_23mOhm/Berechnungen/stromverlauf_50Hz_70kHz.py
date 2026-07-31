"""Berechnet den Phasenstrom aus 50-Hz-Grundwelle und 70-kHz-Dreiecksrippel."""

import numpy as np
import matplotlib.pyplot as plt

F_NET = 50.0
F_SW = 70_000.0
U_LL_RMS = 400.0
U_DC = 750.0

I_STUETZ = np.array([0.0, 28.9, 40.8, 57.7, 81.6, 90.0])
LDIFF_STUETZ = 1e-6 * np.array([525.0, 364.0, 280.0, 202.0, 135.0, 119.0])

LASTFAELLE = {
    "20 kW, 28,87 A RMS": 28.87,
    "40 kW, 57,74 A RMS": 57.74,
}

dt = 1.0 / (F_SW * 100.0)
t = np.arange(0.0, 1.0 / F_NET, dt)

u_phase_peak = np.sqrt(2.0) * U_LL_RMS / np.sqrt(3.0)
u_phase = u_phase_peak * np.sin(2.0 * np.pi * F_NET * t)
d = 0.5 + u_phase / U_DC

phase_sw = (t * F_SW) % 1.0
tri = 4.0 * np.abs(phase_sw - 0.5) - 1.0

for name, i_rms in LASTFAELLE.items():
    i1 = np.sqrt(2.0) * i_rms * np.sin(2.0 * np.pi * F_NET * t)
    ldiff = np.interp(np.abs(i1), I_STUETZ, LDIFF_STUETZ)
    delta_i_pp = U_DC * d * (1.0 - d) / (ldiff * F_SW)
    i_total = i1 + 0.5 * delta_i_pp * tri

    print(
        f"{name}: I1_peak={np.sqrt(2)*i_rms:.3f} A, "
        f"DeltaIpp={delta_i_pp.min():.3f}...{delta_i_pp.max():.3f} A, "
        f"i_min={i_total.min():.3f} A, i_max={i_total.max():.3f} A"
    )

    plt.plot(t * 1e3, i_total, linewidth=0.7, label=name)
    plt.plot(t * 1e3, i1, linestyle="--", linewidth=1.2)

plt.xlabel("Zeit t [ms]")
plt.ylabel("Phasenstrom i(t) [A]")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
