# 7 Magnetische Kennlinien

Die differentielle Induktivität ist für die Stromwelligkeit um den momentanen Arbeitspunkt maßgebend. Die Sekanteninduktivität beschreibt die Flussverkettung bezogen auf den Strom.

## 7.1 B(H)-Kennlinie

![Abbildung 3: B(H)-Kennlinie](Bilder/abbildung_03_bh_kennlinie.svg)

*Abbildung 3: B(H)-Kennlinie für High Flux 60 µ mit den Arbeitspunkten bei 20 kW und 40 kW.*

Der empirische Herstellerfit besitzt bei $H\rightarrow0$ einen kleinen numerischen Offset. Dieser Offset darf nicht direkt zur Berechnung von $L_{sec}=\Psi/I$ verwendet werden, weil andernfalls für kleine Ströme ein unphysikalischer Y-Achsenabschnitt entsteht.

## 7.2 Korrigierte Induktivitätskennlinien

Die differentielle Induktivität wird als lokale Steigung der Flussverkettung verwendet:

$$L_{diff}(I)=\frac{d\Psi}{dI}.$$

Für den Ursprung wird der geometrisch berechnete Anfangswert angesetzt:

$$L_{diff}(0)=L_{sec}(0)=L_0=525\,\mu\mathrm{H}.$$

Die Flussverkettung wird aus der differentiellen Induktivität aufgebaut:

$$\Psi(I)=\int_0^I L_{diff}(i)\,di.$$

Daraus folgt für $I>0$:

$$L_{sec}(I)=\frac{\Psi(I)}{I}=\frac{1}{I}\int_0^I L_{diff}(i)\,di.$$

Damit gilt automatisch der korrekte Grenzwert:

$$\lim_{I\rightarrow0}L_{sec}(I)=L_{diff}(0)=L_0.$$

![Abbildung 4: Differentielle und Sekanteninduktivität](Bilder/abbildung_04_induktivitaet.svg)

*Abbildung 4: Korrigierte differentielle und Sekanteninduktivität. Beide Kennlinien beginnen bei $L_0=525\,\mu\mathrm{H}$. Die Sekanteninduktivität wurde aus der integrierten differentiellen Induktivität berechnet.*

## 7.3 Differentielle und Sekantenpermeabilität

Aus dem Anfangswert $L_0$ und der Anfangspermeabilität $\mu_{r0}=60$ werden die relativen Permeabilitäten konsistent skaliert:

$$\mu_{r,diff}(I)=\mu_{r0}\frac{L_{diff}(I)}{L_0}$$

und

$$\mu_{r,sec}(I)=\mu_{r0}\frac{L_{sec}(I)}{L_0}.$$

![Abbildung 5: Differentielle und Sekantenpermeabilität](Bilder/abbildung_05_permeabilitaet.svg)

*Abbildung 5: Korrigierte differentielle und Sekantenpermeabilität. Beide Kennlinien beginnen bei $\mu_{r0}=60$.*

| Strom | Feldstärke | Flussdichte | $L_{diff}$ | $L_{sec}$ | $\mu_{r,diff}$ | $\mu_{r,sec}$ |
|---:|---:|---:|---:|---:|---:|---:|
| 0,0 A | 0,0 Oe | 0 T | 525 µH | 525 µH | 60,0 | 60,0 |
| 28,9 A | ca. 122 Oe | ca. 0,663 T | 364 µH | ca. 449 µH | 41,6 | ca. 51,3 |
| 40,8 A | ca. 173 Oe | ca. 0,849 T | 280 µH | ca. 412 µH | 32,0 | ca. 47,0 |
| 57,7 A | ca. 243 Oe | ca. 1,038 T | 202 µH | ca. 361 µH | 23,1 | ca. 41,2 |
| 81,6 A | ca. 344 Oe | ca. 1,230 T | 135 µH | ca. 304 µH | 15,4 | ca. 34,7 |
| 90,0 A | ca. 380 Oe | ca. 1,281 T | 119 µH | ca. 287 µH | 13,6 | ca. 32,8 |

Die Werte zwischen den Stützstellen werden monoton interpoliert. Die resultierenden Kennlinien sind eine konsistente Auslegungsnäherung. Die endgültige $L(I)$-Kennlinie ist am Muster von 0 A bis mindestens 90 A zu messen und anschließend in der PLECS-Schaltsimulation zu verwenden.