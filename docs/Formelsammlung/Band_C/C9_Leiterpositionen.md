---
title: "Band C – Leiterpositionen auf dem Ringkern"
chapter: "C9 Ergänzung"
section: "Sechs-Kreis-Modell der zweilagigen Wicklung"
---

# Mathematische Leiterpositionen auf dem Ringkern

## Zweck

Dieses Modell berechnet die Mittelpunkte aller Leiterquerschnitte einer zweilagigen Ringkernwicklung ohne grafische Schätzung. Die Leiter werden als Kreise mit dem Außendurchmesser der gewählten Litze dargestellt.

Für das Beispiel gelten:

| Parameter | Wert |
|---|---:|
| Litze | 630 × 0,10 mm |
| Packungsfaktor | 0,68 |
| berechneter Litzenaußendurchmesser $d_L$ | 3,04 mm |
| Kreis 3, Kern-Innendurchmesser $D_3$ | 35,56 mm |
| Kreis 4, beschichteter Kern-Außendurchmesser $D_4$ | 59,20 mm |
| Leiter auf Kreis 1 und 6 | jeweils 20 |
| Leiter auf Kreis 2 und 5 | jeweils 28 |

## Litzenaußendurchmesser

Der Kupferquerschnitt der Litze ist

```math
A_{Cu}=n\frac{\pi d_E^2}{4}.
```

Mit dem geometrischen Packungsfaktor $\eta_P$ wird der beanspruchte Gesamtquerschnitt

```math
A_L=\frac{A_{Cu}}{\eta_P}.
```

Der äquivalente runde Außendurchmesser lautet damit

```math
d_L=\sqrt{\frac{4A_L}{\pi}}
=d_E\sqrt{\frac{n}{\eta_P}}.
```

Für $n=630$, $d_E=0{,}10\,\mathrm{mm}$ und $\eta_P=0{,}68$ folgt

```math
d_L=0{,}10\sqrt{\frac{630}{0{,}68}}
=3{,}044\,\mathrm{mm}\approx3{,}04\,\mathrm{mm}.
```

## Radien der sechs konzentrischen Kreise

Kreis 3 und Kreis 4 beschreiben die Grenzen des beschichteten Kerns:

```math
r_3=\frac{D_3}{2},\qquad r_4=\frac{D_4}{2}.
```

Die Mittelpunkte der ersten Leiterlage liegen jeweils genau einen halben Leiterdurchmesser von der Kernkontur entfernt:

```math
r_2=r_3-\frac{d_L}{2},
```

```math
r_5=r_4+\frac{d_L}{2}.
```

Die zweite Leiterlage wird radial um genau einen Leiterdurchmesser versetzt:

```math
r_1=r_2-d_L,
```

```math
r_6=r_5+d_L.
```

Daraus folgen die Kreisdurchmesser:

```math
D_k=2r_k.
```

| Kreis | Bedeutung | Radius | Durchmesser | Leiterzahl |
|---:|---|---:|---:|---:|
| 1 | zweite Innenlage | 13,22 mm | 26,44 mm | 20 |
| 2 | erste Innenlage | 16,26 mm | 32,52 mm | 28 |
| 3 | Kern-Innendurchmesser | 17,78 mm | 35,56 mm | – |
| 4 | beschichteter Kern-Außendurchmesser | 29,60 mm | 59,20 mm | – |
| 5 | erste Außenlage | 31,12 mm | 62,24 mm | 28 |
| 6 | zweite Außenlage | 34,16 mm | 68,32 mm | 20 |

## Winkel und kartesische Koordinaten

Für Kreis $k$ mit $N_k$ Leitern wird Leiter $j$ gleichmäßig über den Vollkreis verteilt:

```math
\theta_{k,j}=\theta_{0,k}+\frac{2\pi j}{N_k},
\qquad j=0,1,\ldots,N_k-1.
```

Ohne zusätzlichen Winkelversatz gilt $\theta_{0,k}=0$. Die Mittelpunktkoordinaten lauten

```math
x_{k,j}=r_k\cos(\theta_{k,j}),
```

```math
y_{k,j}=r_k\sin(\theta_{k,j}).
```

Der Leiterquerschnitt an dieser Position ist der Kreis

```math
(x-x_{k,j})^2+(y-y_{k,j})^2
=\left(\frac{d_L}{2}\right)^2.
```

Damit wird nicht nur der Mittelpunkt, sondern auch der Leiteraußendurchmesser maßstäblich dargestellt.

## Winkelabstände

Für die 20-Leiter-Kreise 1 und 6 gilt

```math
\Delta\theta_{20}=\frac{360^\circ}{20}=18^\circ.
```

Für die 28-Leiter-Kreise 2 und 5 gilt

```math
\Delta\theta_{28}=\frac{360^\circ}{28}
=12{,}857142857^\circ.
```

## Reproduzierbare Erzeugung

Das zugehörige Referenzskript ist:

[`C9_Skripte/leiterpositionen_6_kreise.py`](C9_Skripte/leiterpositionen_6_kreise.py)

Es erzeugt eine CSV-Datei mit allen 96 Leiterpositionen sowie eine maßstäbliche SVG- und PNG-Grafik.