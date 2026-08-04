# C9.D DPWMmin-Stromwelligkeit mit variabler differentieller Induktivität

## Zweck

Dieses Kapitel beschreibt die schaltzustandsbasierte Berechnung der hochfrequenten Stromwelligkeit einer dreiphasigen PFC-Drossel bei Minimum-Clamp-SVM. Die Berechnung liefert den zeitlichen Verlauf von $\Delta I_{pp}$ über eine vollständige Netzperiode und berücksichtigt die stromabhängige differentielle Induktivität.

## Minimum-Clamp-SVM

Aus den drei normierten sinusförmigen Phasenreferenzen $m_a$, $m_b$ und $m_c$ wird die Nullsystemkomponente

$$m_{cm}=-1-\min(m_a,m_b,m_c)$$

gebildet. Damit wird der kleinste Phasenreferenzwert auf den negativen Zwischenkreis geklemmt:

$$m_{x,DPWMmin}=m_x+m_{cm}.$$

Die Tastverhältnisse sind

$$d_x=\frac{m_{x,DPWMmin}+1}{2}.$$

Jede Phase ist über 120 elektrische Grad geklemmt. Die Raumzeigerebene bleibt in sechs 60-Grad-Sektoren aufgeteilt.

## Stromabhängige Induktivität

Als analytischer Ersatz für die Magnetisierungskennlinie kann

$$B(H)=B_{sat}\tanh\left(\frac{\mu_0\mu_{r0}H}{B_{sat}}\right)$$

verwendet werden. Daraus folgt

$$L_{diff}(I)=L_0\operatorname{sech}^2\left(\frac{\mu_0\mu_{r0}NI}{l_eB_{sat}}\right).$$

Für belastbare Ergebnisse soll das Skript mit einer gemessenen oder digitalisierten Kennlinie betrieben werden. Die CSV-Datei enthält mindestens:

```csv
current_a,ldiff_uh
0,584
40.82,455.4
81.65,238.4
```

Zwischen den Stützstellen wird linear über $|I|$ interpoliert.

## Schaltzustandsbasierte Integration

Bei $f_s=70\,\mathrm{kHz}$ und $f_{Netz}=50\,\mathrm{Hz}$ enthält eine Netzperiode 1400 PWM-Perioden. Jede PWM-Periode wird zeitdiskret aufgelöst. Aus den Schalterzuständen $S_a$, $S_b$ und $S_c$ wird die auf den schwebenden Sternpunkt bezogene Phasenspannung berechnet:

$$u_{conv,x}=U_{DC}\left(S_x-\frac{S_a+S_b+S_c}{3}\right).$$

Nach Abzug des Mittelwerts der jeweiligen PWM-Periode bleibt die Ripple-Spannung. Für Phase A gilt

$$i_{ripple,A}(t)=\int\frac{u_{ripple,A}(t)}{L_{diff}[i_A(\theta)]}\,dt$$

und

$$\Delta I_{pp,A}(\theta)=\max(i_{ripple,A})-\min(i_{ripple,A}).$$

## Referenzparameter Beispiel 3

| Parameter | Wert |
|---|---:|
| Netzspannung | 400 V Leiter-Leiter |
| Zwischenkreisspannung | 750 V |
| Netzfrequenz | 50 Hz |
| Schaltfrequenz | 70 kHz |
| Anfangsinduktivität | 584 µH |
| Windungszahl | 80 |
| effektive Weglänge | 127 mm |
| Anfangspermeabilität | 26 |
| Modellwert $B_{sat}$ | 1,65 T |

## Ergebnisse mit der Tanh-Entwurfsnäherung

| Arbeitspunkt | $I_{RMS}$ | $L_{diff,min}$ | $\Delta I_{pp,max}$ |
|---|---:|---:|---:|
| Leerlauf | 0 A | 584,0 µH | 2,78 A |
| 20 kW | 28,87 A | 455,4 µH | 3,57 A |
| 40 kW | 57,74 A | 238,4 µH | 6,82 A |

## Archivierte Dateien

- [`C9_Skripte/dpwmmin_variable_ldiff.py`](C9_Skripte/dpwmmin_variable_ldiff.py)
- [`C9_Skripte/ldiff_kennlinie_template.csv`](C9_Skripte/ldiff_kennlinie_template.csv)

Aufruf mit analytischer Kennlinie:

```bash
python C9_Skripte/dpwmmin_variable_ldiff.py --output dpwmmin_delta_ipp.svg
```

Aufruf mit CSV-Kennlinie:

```bash
python C9_Skripte/dpwmmin_variable_ldiff.py \
  --ldiff-csv C9_Skripte/ldiff_kennlinie_template.csv \
  --output dpwmmin_delta_ipp.svg
```

## Modellgrenzen

- Totzeiten und Halbleiterspannungsabfälle sind nicht enthalten.
- Die Grundwellenströme werden sinusförmig und mit Leistungsfaktor eins angesetzt.
- Die lokale Änderung von $L_{diff}$ durch den hochfrequenten Ripple innerhalb einer PWM-Periode wird vernachlässigt.
- Die Tanh-Gleichung ist eine Entwurfsnäherung. Für die Freigabe sind Herstellerdaten oder Messwerte zu verwenden.
- Ein Vergleich mit der konkreten PLECS-Modulatorimplementierung und Messungen am Muster bleibt erforderlich.
