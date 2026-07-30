# 8 Stromwelligkeit und Flussdichtehub

## 8.1 Phasenspannung und Tastgrad

Für das vereinfachte phasenbezogene Zweilevel-PWM-Modell gilt bei sinusförmiger Netzphasenspannung

$$u_{Phase}(\varphi)=\hat U_{Phase}\sin(\varphi)$$

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

*Abbildung 7: Verbesserte Berechnung von $\Delta I_{pp}$ über dem Netzwinkel. Die Kurven für 20 kW und 40 kW verwenden $L_{diff}$ als Funktion des momentanen Grundwellenstroms; die gestrichelte Vergleichskurve zeigt die Rechnung mit konstanten $525\,\mu\mathrm{H}$.*

Mit der derzeit verwendeten $L_{diff}(I)$-Kennlinie ergeben sich:

- 20 kW: $\Delta I_{pp}=2{,}31$ bis $9{,}60\,\mathrm{A}$,
- 40 kW: $\Delta I_{pp}=4{,}79$ bis $9{,}60\,\mathrm{A}$.

Der hohe Rechenwert am Nulldurchgang entsteht aus dem im empirischen B(H)-Fit enthaltenen niedrigen Wert von $L_{diff}(0)\approx279\,\mu\mathrm{H}$. Wie bereits in Kapitel 7 erläutert, besitzt der Fit im Bereich $H\rightarrow0$ einen Offset. Dieser Teil der Kurve muss später durch Messwerte oder ein validiertes Kleinsignalmodell ersetzt werden.

## 8.4 Flussdichtehub aus den beiden verbesserten Stromwelligkeitskurven

Für jeden Netzwinkel wird der Flussdichtehub aus der jeweils verwendeten differentiellen Induktivität und der zugehörigen Stromwelligkeit berechnet:

$$\Delta B_{pp}(\varphi)=
\frac{L_{diff}(\varphi)\,\Delta I_{pp}(\varphi)}{N\,A_e}.$$

Setzt man die Gleichung aus Abschnitt 8.3 ein, kürzt sich $L_{diff}(\varphi)$ vollständig heraus:

$$\Delta B_{pp}(\varphi)=
\frac{U_{DC}\,d(\varphi)\,[1-d(\varphi)]}{N\,A_e\,f_s}.$$

Damit liefern die beiden unterschiedlich großen $\Delta I_{pp}$-Kurven für 20 kW und 40 kW denselben Flussdichtehub. Die Abnahme von $L_{diff}$ erhöht die Stromwelligkeit genau in dem Maß, dass das Produkt $L_{diff}\,\Delta I_{pp}$ durch die vorgegebenen PWM-Voltsekunden unverändert bleibt.

Mit

- $U_{DC}=750\,\mathrm{V}$,
- $N=48$,
- $A_e=432\,\mathrm{mm^2}=432\cdot10^{-6}\,\mathrm{m^2}$,
- $f_s=70\,\mathrm{kHz}$

folgt am Nulldurchgang der Phasenspannung bei $d=0{,}5$:

$$\Delta B_{pp,max}
=\frac{750\cdot0{,}25}{48\cdot432\cdot10^{-6}\cdot70\cdot10^3}
=129{,}175\,\mathrm{mT}.$$

Am Scheitelpunkt der Phasenspannung mit $d=0{,}0645$ beziehungsweise $d=0{,}9355$ ergibt sich:

$$\Delta B_{pp,min}=31{,}193\,\mathrm{mT}.$$

Für beide Betriebspunkte gilt somit identisch:

| Betriebspunkt | $\Delta B_{pp,min}$ | $\Delta B_{pp,max}$ | $B_{pk,min}=\Delta B_{pp,min}/2$ | $B_{pk,max}=\Delta B_{pp,max}/2$ |
|---|---:|---:|---:|---:|
| 20 kW | 31,193 mT | 129,175 mT | 15,597 mT | 64,587 mT |
| 40 kW | 31,193 mT | 129,175 mT | 15,597 mT | 64,587 mT |

![Abbildung 8: Flussdichtehub aus den Ldiff-basierten Stromwelligkeitskurven](Bilder/abbildung_12_flussdichtehub_ldiff_grundwelle.svg)

*Abbildung 8: Aus den beiden verbesserten $\Delta I_{pp}$-Kurven zurückgerechneter Flussdichtehub. Die Kurven für 20 kW und 40 kW sind deckungsgleich, weil der Flussdichtehub bei konsistenter Rechnung ausschließlich von den PWM-Voltsekunden, der Windungszahl, dem Kernquerschnitt und der Schaltfrequenz bestimmt wird.*

## 8.5 Konsequenz für die Kernverlustberechnung

Da $B_{pk}(\varphi)=\Delta B_{pp}(\varphi)/2$ für 20 kW und 40 kW identisch ist, entstehen im bisher verwendeten Steinmetz-Modell auch identische Kernverlustkurven. Das Modell berücksichtigt in dieser Form keinen zusätzlichen Einfluss der DC-Vormagnetisierung auf die Steinmetz-Parameter.

Die dargestellten Werte beruhen weiterhin auf dem vereinfachten phasenbezogenen Zweilevel-PWM-Modell. Für die endgültige Auslegung ist der reale Voltsekunden- und Flussdichteverlauf aus der PLECS-Schaltsimulation zu verwenden. Zusätzlich ist zu prüfen, ob Herstellerkennfelder oder ein iGSE-Modell eine Abhängigkeit der Kernverluste von der DC-Vormagnetisierung berücksichtigen müssen.