# 6 Berechnungsgrundlagen

## 6.1 Anfangsinduktivität

Der AL-basierte Anfangswert beträgt näherungsweise:

$$L_0 \approx 525\,\mu\mathrm{H}$$

## 6.2 Feldstärke

$$H[\mathrm{Oe}] = \frac{4\pi N I}{l_e[\mathrm{mm}]}$$

mit $N=48$ und $l_e=143\,\mathrm{mm}$.

## 6.3 Flussdichtehub

Für das verwendete vereinfachte phasenbezogene Zweilevel-PWM-Modell gilt:

$$d=0{,}5+\frac{v_{Phase}}{V_{DC}}$$

$$\Delta B_{pp}=\frac{V_{DC}\,d(1-d)}{N\,A_e\,f_s}$$

SVM-Gleichtaktanteile, Totzeiten und diskrete Schaltzustände sind in dieser Näherung nicht enthalten.