# 6 Berechnungsgrundlagen

## 6.1 Anfangsinduktivität

Die Anfangsinduktivität wird aus der magnetischen Reluktanz berechnet:

$$L_0=\frac{\mu_0\mu_{r0}N^2A_e}{l_e}.$$

Mit

- $\mu_0=4\pi\cdot10^{-7}\,\mathrm{H/m}$,
- $\mu_{r0}=26$,
- $N=80$,
- $A_e=375\,\mathrm{mm^2}=375\cdot10^{-6}\,\mathrm{m^2}$,
- $l_e=127\,\mathrm{mm}=0{,}127\,\mathrm{m}$

folgt

$$L_0=\frac{4\pi\cdot10^{-7}\cdot26\cdot80^2\cdot375\cdot10^{-6}}{0{,}127}
\approx617\,\mu\mathrm{H}.$$

Der aus den gerundeten Geometrie- und Anfangspermeabilitätswerten berechnete Idealwert liegt damit oberhalb des Projektzielwerts von 584 µH. Für Simulation und Spezifikation ist deshalb die Herstellerkennlinie beziehungsweise der gemessene Anfangswert maßgebend. Die Abweichung ist vor Freigabe am realen Kernstapel zu verifizieren.

## 6.2 Feldstärke

$$H[\mathrm{Oe}]=\frac{4\pi NI}{l_e[\mathrm{mm}]}$$

mit $N=80$ und $l_e=127\,\mathrm{mm}$.

## 6.3 Flussdichtehub

Für das vereinfachte phasenbezogene Zweilevel-PWM-Modell gilt

$$d=0{,}5+\frac{v_{Phase}}{V_{DC}}$$

und

$$\Delta B_{pp}=\frac{V_{DC}d(1-d)}{NA_ef_s}.$$

SVM-Gleichtaktanteile, Totzeiten und diskrete Schaltzustände sind in dieser Näherung nicht enthalten.
