# 8 Stromwelligkeit und Flussdichtehub

## 8.1 Phasenspannung und Tastgrad

Für das vereinfachte phasenbezogene Zweilevel-PWM-Modell gilt bei sinusförmiger Netzphasenspannung

$$
u_{Phase}(\varphi)=\hat U_{Phase}\sin(\varphi)$$

mit

$$\hat U_{Phase}=\frac{\sqrt{2}\,U_{LL,rms}}{\sqrt{3}}
=\frac{\sqrt{2}\cdot400\,\mathrm{V}}{\sqrt{3}}
=326{,}6\,\mathrm{V}.$$

Der zugehörige Tastgrad lautet

$$d(\varphi)=0{,}5+\frac{u_{Phase}(\varphi)}{U_{DC}}.$$

Bei $U_{DC}=750\,\mathrm{V}$ ergibt sich

$$d_{min}=0{,}5-\frac{326{,}6}{750}=0{,}0645$$

und

$$d_{max}=0{,}5+\frac{326{,}6}{750}=0{,}9355.$$

## 8.2 Stromwelligkeit bei konstanter Anfangsinduktivität

Während eines PWM-Zyklus steigt und fällt der Drosselstrom näherungsweise linear. Aus der Voltsekundenbilanz ergibt sich für die Spitze-Spitze-Stromwelligkeit

$$\Delta I_{pp}(\varphi)=\frac{U_{DC}\,d(\varphi)\,[1-d(\varphi)]}{L\,f_s}.$$

Für die Übersicht wird zunächst die Anfangsinduktivität

$$L=L_0=525\,\mu\mathrm{H}$$

verwendet. Bei $d=0{,}5$ ist das Produkt $d(1-d)$ maximal:

$$\Delta I_{pp,max}
=\frac{750\cdot0{,}25}{525\cdot10^{-6}\cdot70\cdot10^3}
=5{,}10\,\mathrm{A}.$$

Am Scheitelpunkt der Netzphasenspannung gilt $d=0{,}0645$ beziehungsweise $d=0{,}9355$:

$$\Delta I_{pp,min}
=\frac{750\cdot0{,}0645\cdot(1-0{,}0645)}{525\cdot10^{-6}\cdot70\cdot10^3}
=1{,}23\,\mathrm{A}.$$

![Abbildung 6: Stromwelligkeit über einer Netzperiode](Bilder/abbildung_10_stromwelligkeit_netzwinkel.svg)

*Abbildung 6: Berechnete Spitze-Spitze-Stromwelligkeit über dem Netzwinkel bei konstanter Anfangsinduktivität von $525\,\mu\mathrm{H}$. Die Maxima liegen an den Nulldurchgängen der Phasenspannung, die Minima an deren Scheitelpunkten.*

## 8.3 Verbesserte Stromwelligkeit mit $L_{diff}$ als Funktion der Grundwelle

Für die realistischere Berechnung wird der momentane Grundwellenstrom als Arbeitspunkt der differentiellen Induktivität verwendet:

$$i_1(\varphi)=\hat I_1\sin(\varphi).$$

Damit folgt

$$L_{diff}(\varphi)=L_{diff}\!\left(\left|i_1(\varphi)\right|\right)$$

und schließlich

$$\Delta I_{pp}(\varphi)=
\frac{U_{DC}\,d(\varphi)\,[1-d(\varphi)]}
{L_{diff}\!\left(\left|\hat I_1\sin(\varphi)\right|\right)\,f_s}.$$

Für das Diagramm wurden die in Kapitel 7 angegebenen Stützstellen der differentiellen Induktivität verwendet und zwischen den Stützstellen linear interpoliert:

| Strom | $L_{diff}$ |
|---:|---:|
| 0,0 A | 279 µH |
| 28,9 A | 364 µH |
| 40,8 A | 280 µH |
| 57,7 A | 202 µH |
| 81,6 A | 135 µH |
| 90,0 A | 119 µH |

Dargestellt sind zwei Betriebspunkte:

- 20 kW mit $\hat I_1=40{,}8\,\mathrm{A}$,
- 40 kW mit $\hat I_1=81{,}6\,\mathrm{A}$.

![Abbildung 7: Stromwelligkeit mit differentieller Induktivität](Bilder/abbildung_11_stromwelligkeit_ldiff_grundwelle.svg)

*Abbildung 7: Verbesserte Berechnung von $\Delta I_{pp}$ über dem Netzwinkel. Die blaue und rote Kurve verwenden $L_{diff}$ als Funktion des momentanen Grundwellenstroms; die gestrichelte Kurve zeigt zum Vergleich die bisherige Rechnung mit konstanten $525\,\mu\mathrm{H}$.*

Die Rechnung zeigt, dass die Annahme einer konstanten Anfangsinduktivität die Stromwelligkeit deutlich unterschätzt. Mit der derzeit verwendeten $L_{diff}(I)$-Kennlinie ergeben sich ungefähr folgende Bereiche:

- 20 kW: $\Delta I_{pp}\approx2{,}3$ bis $9{,}6\,\mathrm{A}$,
- 40 kW: $\Delta I_{pp}\approx4{,}8$ bis $9{,}6\,\mathrm{A}$.

Der hohe Rechenwert am Nulldurchgang entsteht aus dem im empirischen B(H)-Fit enthaltenen niedrigen Wert von $L_{diff}(0)\approx279\,\mu\mathrm{H}$. Wie bereits in Kapitel 7 erläutert, besitzt der Fit im Bereich $H\rightarrow0$ einen Offset. Dieser Teil der Kurve muss daher später durch Messwerte oder ein validiertes Kleinsignalmodell ersetzt werden. Das Diagramm ist eine verbesserte arbeitspunktabhängige Abschätzung, aber noch kein Ersatz für die PLECS-Schaltsimulation.

## 8.4 Flussdichtehub

Aus dem Induktionsgesetz folgt unmittelbar

$$\Delta B_{pp}(\varphi)=\frac{U_{DC}\,d(\varphi)\,[1-d(\varphi)]}{N\,A_e\,f_s}.$$

Mit

- $U_{DC}=750\,\mathrm{V}$,
- $N=48$,
- $A_e=432\,\mathrm{mm^2}=432\cdot10^{-6}\,\mathrm{m^2}$,
- $f_s=70\,\mathrm{kHz}$

ergibt sich am Nulldurchgang der Phasenspannung bei $d=0{,}5$:

$$\Delta B_{pp,max}
=\frac{750\cdot0{,}25}{48\cdot432\cdot10^{-6}\cdot70\cdot10^3}
=0{,}1292\,\mathrm{T}
=129{,}2\,\mathrm{mT}.$$

Am Scheitelpunkt der Phasenspannung gilt $d=0{,}0645$ beziehungsweise $0{,}9355$:

$$\Delta B_{pp,min}
=\frac{750\cdot0{,}0645\cdot(1-0{,}0645)}{48\cdot432\cdot10^{-6}\cdot70\cdot10^3}
=0{,}0312\,\mathrm{T}
=31{,}2\,\mathrm{mT}.$$

Damit ergibt sich über eine Netzperiode:

- $\Delta B_{pp}=31{,}2$ bis $129{,}2\,\mathrm{mT}$,
- $B_{pk}=\Delta B_{pp}/2=15{,}6$ bis $64{,}6\,\mathrm{mT}$.

![Abbildung 8: Flussdichtehub über einer Netzperiode](Bilder/abbildung_01_flussdichtehub_netzwinkel.svg)

*Abbildung 8: Berechneter Flussdichtehub über dem Netzwinkel für den Aufbau mit 48 Windungen und $A_e=432\,\mathrm{mm^2}$.*

## 8.5 Zusammenhang zwischen Strom- und Flussdichtewelligkeit

Bei Verwendung derselben differentiellen Induktivität besteht der Zusammenhang

$$\Delta B_{pp}=\frac{L_{diff}(I)\,\Delta I_{pp}}{N\,A_e}.$$

Der Flussdichtehub wird durch die angelegten Voltsekunden, Windungszahl und Kernquerschnitt bestimmt. Die Stromwelligkeit hängt zusätzlich von der differentiellen Induktivität ab. Eine abnehmende Induktivität erhöht daher $\Delta I_{pp}$, während der durch die PWM-Voltsekunden vorgegebene $\Delta B_{pp}$ unverändert bleibt.

Die dargestellten Werte beruhen auf dem vereinfachten phasenbezogenen Zweilevel-PWM-Modell. Für die endgültige Auslegung ist der reale Voltsekundenverlauf aus der PLECS-Schaltsimulation zu verwenden.
