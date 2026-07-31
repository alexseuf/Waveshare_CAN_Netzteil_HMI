# 9 Elektrische Wicklungsverluste

## 9.1 DC-Wicklungswiderstand

Für die dokumentierte Wicklung aus 630 × 0,10-mm-HF-Litze wird der Gleichstromwiderstand bei 25 °C mit

$$
R_{DC,25}=23{,}20\,\mathrm{m\Omega}
$$

angesetzt. Die Temperaturabhängigkeit des Kupferwiderstands wird mit dem Temperaturkoeffizienten

$$
\alpha_{Cu}=0{,}00393\,\mathrm{K^{-1}}
$$

berechnet:

$$
R(T)=R_{25}\left[1+\alpha_{Cu}(T-25\,^{\circ}\mathrm C)\right].
$$

Für 120 °C folgt damit:

$$
R_{DC,120}=23{,}20\,\mathrm{m\Omega}\cdot\left[1+0{,}00393\cdot(120-25)\right]
=31{,}86\,\mathrm{m\Omega}.
$$

| Wicklungstemperatur | DC-Widerstand |
|---:|---:|
| 25 °C | 23,20 mΩ |
| 120 °C | 31,86 mΩ |

## 9.2 DC-Kupferverluste

Die DC-Kupferverluste ergeben sich aus

$$P_{Cu}=I_{RMS}^2R_{DC}.$$

| Betrieb | Strom | $P_{Cu}$ bei 25 °C | $P_{Cu}$ bei 120 °C |
|---|---:|---:|---:|
| 20 kW | 28,87 A | 19,3 W | 26,6 W |
| 40 kW | 57,74 A | 77,3 W | 106,2 W |

![Abbildung 7: DC-Kupferverluste](Bilder/abbildung_06_kupferverluste.svg)

*Abbildung 7: DC-Kupferverluste über dem Effektivstrom bei 25 °C und 120 °C.*

HF-Zusatzverluste durch Skin- und Proximity-Effekt sind in diesen DC-Werten nicht enthalten. Nach der Formelsammlung ist bei der zweilagigen Wicklung mit einem zusätzlichen AC-Widerstandsfaktor zu rechnen; eine belastbare Freigabe erfordert Messung, harmonische Zerlegung oder FEM.
