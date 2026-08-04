# 8 Stromwelligkeit und Flussdichtehub

## 8.1 Grundgleichungen

Für eine lokale Schaltperiode wird die Stromwelligkeit aus der während der einzelnen Schaltzustände anliegenden Drosselspannung berechnet:

$$\frac{di}{dt}=\frac{u_L}{L_{diff}(i)}.$$

Die vereinfachte Abschätzung

$$\Delta I_{pp}\approx\frac{V_L}{L_{diff}f_s}$$

ist nur dann ausreichend, wenn die Schaltzustandsfolge und die lokale differentielle Induktivität bereits bekannt sind. Für Minimum-Clamp-SVM wird deshalb die tatsächliche Schaltfolge innerhalb jeder PWM-Periode zeitdiskret ausgewertet.

Der Flussdichtehub folgt aus

$$\Delta B_{pp}=\frac{1}{NA_e}\int u_L\,dt.$$

## 8.2 Minimum-Clamp-SVM (DPWMmin)

Die drei sinusförmigen normierten Phasenreferenzen werden mit einer gemeinsamen Nullsystemkomponente verschoben. Für DPWMmin wird der kleinste Referenzwert auf den negativen Zwischenkreis geklemmt:

$$m_{cm}(\theta)=-1-\min\{m_a(\theta),m_b(\theta),m_c(\theta)\}.$$

Die verschobenen Referenzen lauten

$$m_{x,DPWMmin}=m_x+m_{cm}$$

und die Tastverhältnisse

$$d_x=\frac{m_{x,DPWMmin}+1}{2}.$$

Eine Phase bleibt jeweils über 120 elektrische Grad am negativen Zwischenkreis geklemmt. Die Raumzeigerebene besitzt weiterhin sechs Sektoren mit jeweils 60 Grad.

## 8.3 Stromabhängige differentielle Induktivität

Für den dokumentierten Entwurfsstand wird die glatte Magnetisierungsnäherung

$$B(H)=B_{sat}\tanh\left(\frac{\mu_0\mu_{r0}H}{B_{sat}}\right)$$

verwendet. Daraus folgt für die differentielle Induktivität

$$L_{diff}(I)=L_0\operatorname{sech}^2\left(\frac{\mu_0\mu_{r0}NI}{l_eB_{sat}}\right).$$

Eingesetzte Werte:

| Parameter | Wert |
|---|---:|
| $L_0$ | 584 µH |
| $\mu_{r0}$ | 26 |
| $B_{sat}$ | 1,65 T |
| $N$ | 80 |
| $l_e$ | 127 mm |
| $U_{LL,rms}$ | 400 V |
| $U_{DC}$ | 750 V |
| $f_{Netz}$ | 50 Hz |
| $f_s$ | 70 kHz |

Das archivierte Python-Skript kann alternativ eine gemessene oder aus dem Herstellerdiagramm digitalisierte $L_{diff}(I)$-Kennlinie aus einer CSV-Datei einlesen.

## 8.4 Schaltzustandsbasierte Berechnung

Eine 50-Hz-Netzperiode enthält bei 70 kHz insgesamt

$$N_{PWM}=\frac{f_s}{f_{Netz}}=1400$$

PWM-Perioden. Jede PWM-Periode wird im Referenzskript mit 240 Zeitschritten aufgelöst. Aus den drei Schaltzuständen werden die auf den schwebenden Sternpunkt bezogenen Phasenspannungen berechnet. Nach Abzug des Mittelwerts der jeweiligen PWM-Periode wird die hochfrequente Spannung der Phase A integriert:

$$i_{ripple,A}(t)=\int\frac{u_{L,A,ripple}(t)}{L_{diff}[i_A(\theta)]}\,dt.$$

Die Spitze-Spitze-Stromwelligkeit ist

$$\Delta I_{pp,A}(\theta)=\max(i_{ripple,A})-\min(i_{ripple,A}).$$

## 8.5 Ergebnis für Leerlauf, 20 kW und 40 kW

![Abbildung 7a: DPWMmin-Stromwelligkeit der Phase A mit variabler Induktivität](Bilder/abbildung_07a_dpwmmin_stromwelligkeit_variable_induktivitaet.svg)

*Abbildung 7a: Schaltzustandsbasiert berechnete Stromwelligkeit der Phase A über eine vollständige Netzperiode für Leerlauf, 20 kW und 40 kW. Die differentielle Induktivität wird aus dem momentanen sinusförmigen Grundwellenstrom bestimmt.*

| Arbeitspunkt | $I_{RMS}$ | $I_{pk}$ | minimales $L_{diff}$ | maximales $\Delta I_{pp,A}$ |
|---|---:|---:|---:|---:|
| Leerlauf | 0 A | 0 A | 584,0 µH | 2,78 A |
| 20 kW | 28,87 A | 40,82 A | 455,4 µH | 3,57 A |
| 40 kW | 57,74 A | 81,65 A | 238,4 µH | 6,82 A |

Die Maxima treten für Phase A ungefähr bei 90 Grad und 270 Grad auf. Dort erreicht der Grundwellenstrom seinen größten Betrag und $L_{diff}$ seinen kleinsten Wert. Die Knicke und lokalen Minima entstehen aus dem Wechsel der geklemmten Phase und der DPWMmin-Schaltzustandsfolge.

## 8.6 Flussdichtehub

![Abbildung 8: Flussdichtehub](Bilder/abbildung_08_flussdichtehub.svg)

*Abbildung 8: Berechneter hochfrequenter Flussdichtehub über einer Netzperiode.*

Bei identischer Schaltspannungsfolge kürzt sich $L_{diff}$ aus der Beziehung zwischen Stromwelligkeit und Flussdichtehub heraus. Der Flussdichtehub wird daher direkt aus dem Spannungs-Zeit-Integral bestimmt. Für die endgültige Freigabe sind Totzeiten, reale Halbleiterspannungsabfälle, Reglerkorrekturen und die gemessene beziehungsweise digitalisierte Kernkennlinie zu berücksichtigen.

## 8.7 Reproduzierbarkeit

Die vollständige Referenzimplementierung ist in der Formelsammlung archiviert:

- [C9 – Reproduzierbare Diagrammerzeugung](../../../Formelsammlung/Band_C/C9.md)
- [`dpwmmin_variable_ldiff.py`](../../../Formelsammlung/Band_C/C9_Skripte/dpwmmin_variable_ldiff.py)
- [`ldiff_kennlinie_template.csv`](../../../Formelsammlung/Band_C/C9_Skripte/ldiff_kennlinie_template.csv)
