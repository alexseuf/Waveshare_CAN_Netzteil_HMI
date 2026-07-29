# 9 Elektrische Wicklungsverluste

## 9.1 DC-Widerstand

```math
R(T)=R_{25}\left[1+\alpha_\mathrm{Cu}(T-25\,^{\circ}\mathrm C)\right]
```

mit $\alpha_\mathrm{Cu}\approx 0{,}00393\,\mathrm{K^{-1}}$.

| Temperatur | DC-Widerstand | Bemerkung |
|---:|---:|---|
| 25 °C |  | gesamte Wicklung inkl. Anschlüsse |
| 120 °C |  | berechnet oder gemessen |

## 9.2 AC-Widerstand

```math
k_\mathrm{AC}=\frac{R_\mathrm{AC}}{R_\mathrm{DC}}
```

```math
R_\mathrm{AC}=k_\mathrm{AC}R_\mathrm{DC}
```

Der Faktor $k_\mathrm{AC}$ berücksichtigt Zusatzverluste durch Skin- und Proximity-Effekt. Für eine Vorabschätzung ist der verwendete Praxisfaktor zu dokumentieren; für die Freigabe sind Berechnung, FEM oder Messung vorzuziehen.

| Betriebsfall | $k_\mathrm{AC}$ | Begründung |
|---|---:|---|
| Grundwelle |  |  |
| Schaltwelligkeit |  |  |
| konservative Gesamtabschätzung |  |  |

## 9.3 Kupferverlust

```math
P_\mathrm{Cu}=I_\mathrm{RMS}^2R_\mathrm{AC}
```

| Arbeitspunkt | Strom | $P_\mathrm{Cu}$ bei 25 °C | $P_\mathrm{Cu}$ bei Betriebstemperatur |
|---|---:|---:|---:|
| Nennbetrieb |  |  |  |
| Überlast |  |  |  |

![Kupferverluste](Bilder/Kupferverluste.png)
