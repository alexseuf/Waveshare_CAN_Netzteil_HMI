---
title: "PFC-Drossel 525 µH – Gesamtausgabe"
version: "Revision 4.6"
last_updated: "2026-07-30 09:30 CEST"
---

# PFC-Drossel 525 µH – Gesamtausgabe

**3 × Magnetics C058110A2 · High Flux 60 µ · 48 Windungen · 28 + 20 · Litze 630 × 0,10 mm**

Diese Datei fasst alle Einzelkapitel des Beispielprojekts in einer fortlaufenden Leseansicht zusammen.

## Inhaltsverzeichnis

1. [Zweck und Geltungsbereich](#1-zweck-und-geltungsbereich)
2. [Systemanforderungen](#2-systemanforderungen)
3. [Magnetischer Aufbau](#3-magnetischer-aufbau)
4. [Wicklungsaufbau](#4-wicklungsaufbau)
5. [Mechanischer Aufbau](#5-mechanischer-aufbau)
6. [Berechnungsgrundlagen](#6-berechnungsgrundlagen)
7. [Magnetische Kennlinien](#7-magnetische-kennlinien)
8. [Stromwelligkeit und Flussdichtehub](#8-stromwelligkeit-und-flussdichtehub)
9. [Elektrische Wicklungsverluste](#9-elektrische-wicklungsverluste)
10. [Kernverluste](#10-kernverluste)
11. [Gesamtverluste](#11-gesamtverluste)
12. [Ausnutzung Kernmaterial](#12-ausnutzung-kernmaterial)
13. [Thermik](#13-thermik)
14. [Fertigung](#14-fertigung)
15. [Prüfung](#15-prüfung)
16. [PLECS-/MATLAB-Parametersatz](#16-plecs-matlab-parametersatz)
17. [Bewertung](#17-bewertung)
18. [Quellen und Rechenunterlagen](#18-quellen-und-rechenunterlagen)

---

# 1 Zweck und Geltungsbereich

Dieses Beispiel dokumentiert den Entwurfsstand einer dreiphasigen PFC-Drossel mit etwa 525 µH Anfangsinduktivität, drei gestapelten Magnetics-C058110A2-Kernen und 48 Windungen HF-Litze.

# 2 Systemanforderungen

| Parameter | Wert |
|---|---:|
| Netzspannung | 400 V Leiter-Leiter |
| Zwischenkreisspannung | 750 V DC |
| Schaltfrequenz | 70 kHz |
| Nennleistung | 20 kW dauerhaft |
| Spitzenleistung | 40 kW für 0,5 s |
| Nennstrom | 28,87 A RMS |
| Spitzenlaststrom | 57,74 A RMS |

# 3 Magnetischer Aufbau

| Parameter | Wert |
|---|---:|
| Kern | 3 × Magnetics C058110A2 |
| Material | High Flux, µ = 60 |
| Außendurchmesser | 57,15 mm |
| Innendurchmesser | 35,56 mm |
| Höhe je Kern | 13,97 mm |
| Gesamthöhe | 41,91 mm |
| Effektiver Querschnitt | 432 mm² |
| Effektive Weglänge | 143 mm |
| Kernvolumen | 61,78 cm³ |
| Modellwert $B_{sat}$ | 1,78 T |

# 4 Wicklungsaufbau

| Parameter | Wert |
|---|---:|
| Windungszahl | 48 |
| Lagenaufteilung | 28 + 20 |
| Lagenzahl | 2 |
| Litze | 630 × 0,10 mm |
| Kupferquerschnitt | 4,948 mm² |

![Zweilagiger Wicklungsaufbau](Bilder/abbildung_02_wicklungsaufbau.svg)

*Abbildung 1: Zweilagiger Wicklungsaufbau mit 28 Windungen in der ersten und 20 Windungen in der zweiten Lage.*

# 5 Mechanischer Aufbau

Die drei Kerne sind fluchtend zu stapeln, elektrisch isolierend zu verkleben und gegen Beschädigung durch die zweilagige Wicklung zu schützen. Die Anschlüsse sind separat zugzuentlasten.

![Mechanischer Aufbau](Bilder/abbildung_01_mechanischer_aufbau.svg)

*Abbildung 2: Schematischer mechanischer Aufbau der Drossel.*

# 6 Berechnungsgrundlagen

## 6.1 Anfangsinduktivität

$$L_0=\frac{\mu_0\mu_{r0}N^2A_e}{l_e}.$$

Mit $\mu_{r0}=60$, $N=48$, $A_e=432\,\mathrm{mm^2}$ und $l_e=143\,\mathrm{mm}$ ergibt sich

$$L_0\approx525\,\mu\mathrm H.$$

## 6.2 Feldstärke

$$H[\mathrm{Oe}]=\frac{4\pi NI}{l_e[\mathrm{mm}]}.$$

## 6.3 Flussdichtehub

$$d=0{,}5+\frac{v_{Phase}}{V_{DC}}$$

$$\Delta B_{pp}=\frac{V_{DC}d(1-d)}{NA_ef_s}.$$

# 7 Magnetische Kennlinien

Die differentielle Induktivität ist für die Stromwelligkeit um den momentanen Arbeitspunkt maßgebend. Die Sekanteninduktivität beschreibt die Flussverkettung bezogen auf den Strom.

![B(H)-Kennlinie](Bilder/abbildung_03_bh_kennlinie.svg)

*Abbildung 3: B(H)-Kennlinie des verwendeten High-Flux-Modells.*

Für die korrigierten Induktivitätskennlinien gilt

$$L_{diff}(0)=L_{sec}(0)=L_0=525\,\mu\mathrm H.$$

Die Flussverkettung und die Sekanteninduktivität werden aus der differentiellen Induktivität berechnet:

$$\Psi(I)=\int_0^I L_{diff}(i)\,di$$

$$L_{sec}(I)=\frac{1}{I}\int_0^I L_{diff}(i)\,di.$$

![Differentielle und Sekanteninduktivität](Bilder/abbildung_04_induktivitaet.svg)

*Abbildung 4: Korrigierte differentielle und Sekanteninduktivität. Beide Kennlinien beginnen bei $525\,\mu\mathrm H$.*

$$\mu_{r,diff}(I)=\mu_{r0}\frac{L_{diff}(I)}{L_0}$$

$$\mu_{r,sec}(I)=\mu_{r0}\frac{L_{sec}(I)}{L_0}.$$

![Differentielle und Sekantenpermeabilität](Bilder/abbildung_05_permeabilitaet.svg)

*Abbildung 5: Korrigierte differentielle und Sekantenpermeabilität.*

| Strom | $L_{diff}$ | $L_{sec}$ | $\mu_{r,diff}$ | $\mu_{r,sec}$ |
|---:|---:|---:|---:|---:|
| 0,0 A | 525 µH | 525 µH | 60,0 | 60,0 |
| 28,9 A | 364 µH | ca. 449 µH | 41,6 | ca. 51,3 |
| 40,8 A | 280 µH | ca. 412 µH | 32,0 | ca. 47,0 |
| 57,7 A | 202 µH | ca. 361 µH | 23,1 | ca. 41,2 |
| 81,6 A | 135 µH | ca. 304 µH | 15,4 | ca. 34,7 |
| 90,0 A | 119 µH | ca. 287 µH | 13,6 | ca. 32,8 |

# 8 Stromwelligkeit und Flussdichtehub

## 8.1 Phasenspannung und Tastgrad

$$u_{Phase}(t)=\hat U_{Phase}\sin(2\pi f_{Netz}t)$$

$$\hat U_{Phase}=\frac{\sqrt{2}\,U_{LL,rms}}{\sqrt{3}}=326{,}6\,\mathrm V$$

$$d(t)=0{,}5+\frac{u_{Phase}(t)}{U_{DC}}.$$

## 8.2 Stromwelligkeit mit $L_{diff}$

Der Grundwellenstrom lautet

$$i_1(t)=\sqrt{2}\,I_{1,rms}\sin(2\pi f_{Netz}t).$$

Die arbeitspunktabhängige Spitze-Spitze-Stromwelligkeit ist

$$\Delta I_{pp}(t)=\frac{U_{DC}\,d(t)[1-d(t)]}{L_{diff}(|i_1(t)|)f_s}.$$

Dabei werden die korrigierten Stützstellen $L_{diff}(0)=525\,\mu\mathrm H$ bis $L_{diff}(90\,\mathrm A)=119\,\mu\mathrm H$ linear interpoliert.

![Stromwelligkeit bei konstantem L](Bilder/abbildung_10_stromwelligkeit_netzwinkel.svg)

*Abbildung 6: Vergleichsrechnung mit konstanter Anfangsinduktivität.*

![Stromwelligkeit mit differentieller Induktivität](Bilder/abbildung_11_stromwelligkeit_ldiff_grundwelle.svg)

*Abbildung 7: Arbeitspunktabhängige Stromwelligkeit mit korrigierter differentieller Induktivität.*

| Betriebspunkt | $\hat I_1$ | $\Delta I_{pp,min}$ | $\Delta I_{pp,max}$ |
|---|---:|---:|---:|
| 20 kW, 28,87 A RMS | 40,83 A | 2,31 A | 5,46 A |
| 40 kW, 57,74 A RMS | 81,66 A | 4,80 A | 8,24 A |

Die früher angegebenen $9{,}60\,\mathrm A$ basierten auf dem nicht korrigierten Wert $L_{diff}(0)\approx279\,\mu\mathrm H$ und werden nicht mehr verwendet.

## 8.3 Überlagerung von Grundwelle und Dreiecksrippel

Der normierte symmetrische Dreiecksträger lautet

$$\operatorname{tri}(t)=4\left|\left(f_s t\bmod1\right)-\frac12\right|-1.$$

Der vollständige Phasenstrom wird zeitdiskret berechnet:

$$i(t)=i_1(t)+\frac{\Delta I_{pp}(t)}{2}\operatorname{tri}(t).$$

Die momentanen Grenzen sind

$$i_{oben}(t)=i_1(t)+\frac{\Delta I_{pp}(t)}{2}$$

und

$$i_{unten}(t)=i_1(t)-\frac{\Delta I_{pp}(t)}{2}.$$

![Berechneter Stromverlauf](Bilder/abbildung_09_stromverlauf_50Hz_70kHz.svg)

*Abbildung 8: Numerisch berechnete Überlagerung der 50-Hz-Grundwelle mit dem netzwinkel- und arbeitspunktabhängigen 70-kHz-Dreiecksrippel. Oben sind die berechneten Hüllkurven über eine Netzperiode dargestellt; unten ist der 70-kHz-Verlauf vergrößert.*

| Betriebspunkt | Grundwellen-Scheitel | maximaler Gesamtstrom | minimaler Gesamtstrom |
|---|---:|---:|---:|
| 20 kW | 40,83 A | 41,98 A | −41,98 A |
| 40 kW | 81,66 A | 84,05 A | −84,05 A |

Die Gesamtstrommaxima werden direkt aus der zeitdiskreten Kurve bestimmt. Das Maximum von $\Delta I_{pp}$ und der Scheitel der Grundwelle fallen nicht zwingend zeitlich zusammen.

## 8.4 Flussdichtehub

$$\Delta B_{pp}(t)=\frac{L_{diff}(t)\Delta I_{pp}(t)}{NA_e}$$

und nach Einsetzen

$$\Delta B_{pp}(t)=\frac{U_{DC}d(t)[1-d(t)]}{NA_ef_s}.$$

| Betriebspunkt | $\Delta B_{pp,min}$ | $\Delta B_{pp,max}$ | $B_{pk,min}$ | $B_{pk,max}$ |
|---|---:|---:|---:|---:|
| 20 kW | 31,193 mT | 129,175 mT | 15,597 mT | 64,588 mT |
| 40 kW | 31,193 mT | 129,175 mT | 15,597 mT | 64,588 mT |

![Flussdichtehub](Bilder/abbildung_12_flussdichtehub_ldiff_grundwelle.svg)

*Abbildung 9: Deckungsgleicher Flussdichtehub für beide Lastfälle.*

# 9 Elektrische Wicklungsverluste

| Betrieb | $P_{Cu}$ 25 °C | $P_{Cu}$ 120 °C |
|---|---:|---:|
| 20 kW | 19,3 W | 26,6 W |
| 40 kW | 77,3 W | 106,4 W |

![DC-Kupferverluste](Bilder/abbildung_06_kupferverluste.svg)

*Abbildung 10: DC-Kupferverluste bei 25 °C und 120 °C.*

# 10 Kernverluste

$$P_v[\mathrm{mW/cm^3}]=246{,}54\cdot B[\mathrm T]^{2{,}218}\cdot f[\mathrm{kHz}]^{1{,}311}.$$

| Betriebspunkt | Minimum | Mittelwert | Maximum |
|---|---:|---:|---:|
| 20 kW | 0,392 W | 4,000 W | 9,174 W |
| 40 kW | 0,392 W | 4,000 W | 9,174 W |

![Kernverlust](Bilder/abbildung_07_kernverluste.svg)

*Abbildung 11: Deckungsgleiche Kernverlustkurven für 20 kW und 40 kW.*

# 11 Gesamtverluste

| Betrieb | $P_{Cu}$ 25 °C | $P_{Cu}$ 120 °C | $P_{core}$ | $P_{ges}$ bei 120 °C |
|---|---:|---:|---:|---:|
| 20 kW | 19,3 W | 26,6 W | 4,000 W | 30,600 W |
| 40 kW | 77,3 W | 106,4 W | 4,000 W | 110,400 W |

![Gesamtverluste](Bilder/abbildung_08_gesamtverluste.svg)

*Abbildung 12: Gesamtverluste über dem Effektivstrom.*

# 12 Ausnutzung Kernmaterial

$$\hat I_1=\sqrt{2}\,I_{1,rms}$$

$$\Delta B_{pk,max}=\frac{129{,}175\,\mathrm{mT}}{2}=64{,}588\,\mathrm{mT}.$$

![Ausnutzung des Kernmaterials](Bilder/abbildung_13_ausnutzung_kernmaterial_bmax.svg)

*Abbildung 13: Maximale Flussdichte der Grundwelle und einschließlich des maximalen PWM-Anteils.*

| Betriebspunkt | $I_{1,rms}$ | $\hat I_1$ | $B_{GW}$ | $B_{gesamt}$ | Ausnutzung $B_{sat}$ |
|---|---:|---:|---:|---:|---:|
| 20 kW | 28,87 A | 40,83 A | 0,849 T | 0,914 T | 51,3 % |
| 40 kW | 57,74 A | 81,66 A | 1,230 T | 1,295 T | 72,7 % |

# 13 Thermik

Für 20 kW Dauerbetrieb beträgt der maximal zulässige thermische Widerstand etwa 3,11 K/W. Mit einer angenommenen Wärmekapazität von 800 J/K ergibt sich während 0,5 s Spitzenlast ein adiabatischer Temperaturanstieg von etwa 0,069 K.

![Adiabatische Temperaturerhöhung](Bilder/abbildung_09_temperaturanstieg.svg)

*Abbildung 14: Adiabatische Temperaturerhöhung während der 0,5-s-Spitzenlast.*

# 14 Fertigung

Kerne prüfen, fluchtend stapeln und isolierend verkleben. Erste Lage mit 28 Windungen wickeln, Lagenisolation aufbringen und zweite Lage mit 20 Windungen ergänzen. Anschlüsse fachgerecht kontaktieren und zugentlasten.

# 15 Prüfung

Zu prüfen sind Sichtzustand, 48 Windungen mit Aufteilung 28 + 20, Anfangsinduktivität, $L(I)$, $R_{DC}$, Isolation, stationärer Temperaturbetrieb und Kurzzeitüberlast.

# 16 PLECS-/MATLAB-Parametersatz

```matlab
D_a_mm = 57.15;
D_i_mm = 35.56;
h_mm = 3*13.97;
N_Wdg = 48;
N_Litze = 630;
d_Litze_mm = 0.10;
A_mag = 3*144e-6;
l_mag_mm = 143;
mu_r_0 = 60;
mu_r_sat = 1;
B_sat = 1.78;
a_StMetz = 246.54;
b_StMetz = 2.218;
c_StMetz = 1.311;
U_LL_rms = 400;
U_DC = 750;
f_sw = 70e3;
```

# 17 Bewertung

Die Kennlinien erfüllen die Randbedingung

$$L_{diff}(0)=L_{sec}(0)=L_0=525\,\mu\mathrm H.$$

Die zeitdiskrete Berechnung liefert für den 20-kW-Lastfall einen maximalen Gesamtstrom von 41,98 A und für den 40-kW-Lastfall 84,05 A. Bei 40 kW ergibt sich einschließlich des maximalen 70-kHz-Flussanteils eine Flussdichte von etwa 1,295 T beziehungsweise 72,7 % der Modell-Sättigungsflussdichte von 1,78 T.

Die endgültige Auslegung ist durch Messungen und den realen PLECS-Flussdichteverlauf zu verifizieren.

# 18 Quellen und Rechenunterlagen

- Entwicklungsspezifikation „Entwicklungsspezifikation_PFC_Drossel_Rev4_4_final“, Revision 4.3.
- Magnetics High Flux 60 µ, Kern C058110A2.
- Projektinterne Formelsammlung zur Drosselauslegung.
- `Berechnungen/stromverlauf_50Hz_70kHz.py`: reproduzierbare Berechnung von Grundwelle, Tastgrad, $L_{diff}$, Dreiecksrippel und Gesamtstrom.
- `Daten/stromverlauf_50Hz_70kHz_kennwerte.csv`: verwendete Stützstellen und numerische Ergebniskennwerte.
