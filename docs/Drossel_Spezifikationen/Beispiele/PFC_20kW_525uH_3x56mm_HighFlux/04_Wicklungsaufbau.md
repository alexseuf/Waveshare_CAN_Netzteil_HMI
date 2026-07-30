# 4 Wicklungsaufbau

| Parameter | Wert |
|---|---:|
| Windungszahl | 48 |
| Lagenaufteilung | 28 + 20 |
| Lagenzahl | 2 |
| Litze | 630 × 0,10 mm |
| Kupferquerschnitt | 4,948 mm² |
| Verwendeter Litzenaußendurchmesser | 3,50 mm |
| Dargestellte Leiterquerschnitte | 2 × 48 = 96 |

Die Wicklung wird zweilagig ausgeführt. Die erste Lage umfasst 28 Windungen, die zweite Lage 20 Windungen. Jede Windung erscheint in der maßstäblichen Draufsicht einmal auf der Innen- und einmal auf der Außenseite des Ringkerns. Dadurch werden insgesamt 96 Leiterquerschnitte dargestellt. Zwischen den Lagen ist eine geeignete Isolier- und Abriebschutzlage vorzusehen.

![Abbildung 2: Zweilagiger Wicklungsaufbau](Bilder/abbildung_02_wicklungsaufbau.svg)

*Abbildung 2: Drauf- und Seitenansicht des zweilagigen Wicklungsaufbaus mit 28 + 20 Windungen auf drei gestapelten Kernen.*

## Mathematisch berechnete Leiterpositionen

Für jeden Leiter auf einem Mittelpunktkreis mit Radius $r_k$ und $N_k$ gleichmäßig verteilten Leitern gilt

```math
\theta_{k,j}=\frac{2\pi j}{N_k},\qquad j=0,1,\ldots,N_k-1,
```

```math
x_{k,j}=r_k\cos(\theta_{k,j}),\qquad
y_{k,j}=r_k\sin(\theta_{k,j}).
```

Der Leiteraußendurchmesser beträgt

```math
d_L=3{,}50\,\mathrm{mm}.
```

Mit dem inneren Kerndurchmesser $D_3=35{,}56\,\mathrm{mm}$ und dem äußeren beschichteten Kerndurchmesser $D_4=59{,}20\,\mathrm{mm}$ gelten die Kernradien

```math
r_3=\frac{D_3}{2}=17{,}780\,\mathrm{mm},
\qquad
r_4=\frac{D_4}{2}=29{,}600\,\mathrm{mm}.
```

Die vier Leiter-Mittelpunktkreise werden berechnet mit

```math
r_2=r_3-\frac{d_L}{2},
\qquad
r_1=r_2-d_L,
```

```math
r_5=r_4+\frac{d_L}{2},
\qquad
r_6=r_5+d_L.
```

Daraus ergeben sich:

| Kreis | Funktion | Leiterzahl | Radius | Durchmesser |
|---:|---|---:|---:|---:|
| 1 | zweite Innenlage | 20 | 12,530 mm | 25,060 mm |
| 2 | erste Innenlage | 28 | 16,030 mm | 32,060 mm |
| 3 | Innendurchmesser des beschichteten Kerns | – | 17,780 mm | 35,560 mm |
| 4 | Außendurchmesser des beschichteten Kerns | – | 29,600 mm | 59,200 mm |
| 5 | erste Außenlage | 28 | 31,350 mm | 62,700 mm |
| 6 | zweite Außenlage | 20 | 34,850 mm | 69,700 mm |

![Abbildung 14: Maßstäbliche Leiterpositionen](Bilder/abbildung_14_leiterpositionen_massstaeblich.svg)

*Abbildung 14: Maßstäbliche Draufsicht der 96 mathematisch berechneten Leiterquerschnitte. Die Mittelpunkte liegen exakt auf den vier berechneten Mittelpunktkreisen; jeder Leiter besitzt einen Außendurchmesser von 3,50 mm.*

Die Datei `Bilder/abbildung_14_leiterpositionen_massstaeblich.svg` ist die verbindliche Referenzdarstellung für diesen Wicklungsaufbau.

Kreuzungen, lokale Bündelungen und scharfkantige Umlenkungen sind zu vermeiden. Anfang und Ende der Wicklung müssen separat zugentlastet werden.
