# 10 Kernverluste

Für High Flux 60 µ wird die korrigierte Steinmetz-Konvention aus der Formelsammlung verwendet:

$$P_v[\mathrm{mW/cm^3}]=246{,}54\cdot B[\mathrm{T}]^{2{,}218}\cdot f[\mathrm{kHz}]^{1{,}311}$$

$$P_{core}=P_v\,V_e$$

| Parameter | Wert |
|---|---:|
| $a$ | 246,54 |
| $b$ | 2,218 |
| $c$ | 1,311 |
| Kernvolumen | 61,78 cm³ |
| mittlerer Kernverlust | 4,0 W |
| maximaler momentaner Rechenwert | 9,2 W |

![Abbildung 8: Kernverlust über einer Netzperiode](Bilder/abbildung_07_kernverluste.svg)

*Abbildung 8: Momentaner Kernverlust über einer Netzperiode mit einem Mittelwert von 4,0 W.*

In dieser Fassung werden Flussdichte in Tesla und Frequenz in kHz eingesetzt. Entsprechend der Formelsammlung ist für die nichtsinusförmige PWM-Anregung eine Verifikation mit iGSE, Herstellerkennfeldern oder dem realen PLECS-Flussdichteverlauf vorzuziehen.