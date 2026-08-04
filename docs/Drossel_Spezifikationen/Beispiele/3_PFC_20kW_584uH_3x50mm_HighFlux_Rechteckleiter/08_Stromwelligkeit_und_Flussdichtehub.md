# 8 Stromwelligkeit und Flussdichtehub

Für eine lokale Schaltperiode wird die Stromwelligkeit näherungsweise mit

$$\Delta I_{pp}=\frac{V_L}{L_{diff}f_s}$$

berechnet. Wegen der stromabhängigen Permeabilität ist dabei die differentielle Induktivität am jeweiligen Arbeitspunkt einzusetzen.

Der Flussdichtehub folgt aus

$$\Delta B_{pp}=\frac{V_L}{NA_ef_s}.$$

Die folgenden Kurven wurden über eine vollständige Netzperiode mit dem vereinfachten phasenbezogenen PWM-Modell, $U_{DC}=750\,\mathrm V$, $f_s=70\,\mathrm{kHz}$, $N=80$ und $A_e=375\,\mathrm{mm^2}$ neu berechnet.

![Abbildung 7: Stromwelligkeit](Bilder/abbildung_07_stromwelligkeit.svg)

*Abbildung 7: Berechnete Stromwelligkeit über einer Netzperiode. Die beiden Kurven verwenden die differentiellen Induktivitäten der Arbeitspunkte 20 kW und 40 kW. Im vereinfachten Modell ergeben sich maximal etwa $5{,}9\,\mathrm A_{pp}$ bei 20 kW und $11{,}3\,\mathrm A_{pp}$ bei 40 kW.*

![Abbildung 8: Flussdichtehub](Bilder/abbildung_08_flussdichtehub.svg)

*Abbildung 8: Berechneter hochfrequenter Flussdichtehub über einer Netzperiode. Bei gleichem PWM-Spannungszustand ist $\Delta B_{pp}$ unabhängig vom Laststrom; für beide Arbeitspunkte ergibt sich in dieser Näherung ein Maximum von rund $98\,\mathrm{mT}_{pp}$.*

SVM-Gleichtaktanteile, Totzeiten, diskrete Schaltzustände und die Rückwirkung der Stromwelligkeit auf die lokale differentielle Induktivität sind in dieser Darstellung noch nicht vollständig enthalten.
