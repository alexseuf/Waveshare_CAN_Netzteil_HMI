# 6 Berechnungsgrundlagen

## 6.1 Anfangsinduktivität

Die Anfangsinduktivität wird direkt aus der magnetischen Reluktanz berechnet:

$$L_0=\frac{N^2}{\mathcal{R}_m}$$

mit

$$\mathcal{R}_m=\frac{l_e}{\mu_0\,\mu_{r0}\,A_e}.$$

Durch Einsetzen folgt:

$$L_0=\frac{\mu_0\,\mu_{r0}\,N^2\,A_e}{l_e}.$$

Verwendet werden die Werte aus Kapitel 3 und 4:

- $\mu_0=4\pi\cdot10^{-7}\,\mathrm{H/m}$,
- $\mu_{r0}=60$,
- $N=48$,
- $A_e=432\,\mathrm{mm^2}=432\cdot10^{-6}\,\mathrm{m^2}$,
- $l_e=143\,\mathrm{mm}=0{,}143\,\mathrm{m}$.

Damit ergibt sich:

$$
L_0=
\frac{4\pi\cdot10^{-7}\cdot60\cdot48^2\cdot432\cdot10^{-6}}
{0{,}143}
=5{,}25\cdot10^{-4}\,\mathrm{H}
\approx525\,\mu\mathrm{H}.
$$

Der berechnete Wert stimmt mit der für den Entwurf angesetzten Anfangsinduktivität überein.

## 6.2 Feldstärke

$$H[\mathrm{Oe}] = \frac{4\pi N I}{l_e[\mathrm{mm}]}$$

mit $N=48$ und $l_e=143\,\mathrm{mm}$.

## 6.3 Flussdichtehub

Für das verwendete vereinfachte phasenbezogene Zweilevel-PWM-Modell gilt:

$$d=0{,}5+\frac{v_{Phase}}{V_{DC}}$$

$$\Delta B_{pp}=\frac{V_{DC}\,d(1-d)}{N\,A_e\,f_s}$$

SVM-Gleichtaktanteile, Totzeiten und diskrete Schaltzustände sind in dieser Näherung nicht enthalten.