---
title: "PFC-Drossel 584 µH – 3 × 50,8 mm High Flux – Gesamtausgabe"
version: "Revision 1.1"
last_updated: "2026-08-03"
---

# PFC-Drossel 584 µH – 3 × Magnetics 0058717A2

**High Flux 26 µ · 80 Windungen · Rechteckleiter 0,72 × 4,00 mm · Kupferquerschnitt 2,88 mm²**

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
12. [Thermik](#12-thermik)
13. [Fertigung](#13-fertigung)
14. [Prüfung](#14-prüfung)
15. [PLECS-/MATLAB-Parametersatz](#15-plecs-matlab-parametersatz)
16. [Bewertung](#16-bewertung)
17. [Quellen](#17-quellen)

# 1 Zweck und Geltungsbereich

Dieses Beispiel dokumentiert den Entwurfsstand einer dreiphasigen PFC-Drossel mit einer Zielinduktivität von 584 µH. Der Aufbau besteht aus drei axial gestapelten Magnetics-0058717A2-High-Flux-Ringkernen und 80 Windungen Rechteckkupferleiter 0,72 × 4,00 mm.

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
| Zielinduktivität | 584 µH |

# 3 Magnetischer Aufbau

| Parameter | Wert |
|---|---:|
| Kern | 3 × Magnetics 0058717A2 |
| Material | High Flux, µ = 26 |
| Außendurchmesser | 50,80 mm |
| Innendurchmesser | 31,75 mm |
| Höhe je Kern / gesamt | 13,46 mm / 40,38 mm |
| Effektiver Querschnitt $A_e$ | 375 mm² |
| Effektive Weglänge $l_e$ | 127 mm |
| Modellwert $B_{sat}$ | 1,65 T |

# 4 Wicklungsaufbau

| Parameter | Wert |
|---|---:|
| Windungszahl | 80 |
| Leiter | Rechteckkupfer 0,72 × 4,00 mm |
| Kupferquerschnitt | 2,88 mm² |
| Ersatz-Runddurchmesser | 1,915 mm |

![Abbildung 14: Einlagiger Rechteckleiter-Wicklungsaufbau](Bilder/abbildung_14_wicklungsaufbau_rechteckleiter_einlagig.svg)

# 5 Mechanischer Aufbau

Der dreifache Kernstapel wird fluchtend und elektrisch isolierend verklebt. Kernkanten sind zu schützen; Rechteckleiter dürfen weder scharf geknickt noch lokal eingeschnürt werden.

![Abbildung 1: Mechanischer Aufbau](Bilder/abbildung_01_mechanischer_aufbau.svg)

# 6 Berechnungsgrundlagen

$$L_0=\frac{\mu_0\mu_{r0}N^2A_e}{l_e}.$$

Mit den gerundeten Geometriedaten ergibt die lineare Reluktanzformel etwa 617 µH. Für Spezifikation und Simulation wird der Projektwert 584 µH verwendet; der Abgleich ist am realen Kernstapel durchzuführen.

# 7 Magnetische Kennlinien

Die neu berechneten Kennlinien verwenden die glatte Sättigungsnäherung

$$B(H)=B_{sat}\tanh\left(\frac{\mu_0\mu_{r0}H}{B_{sat}}\right)$$

mit $\mu_{r0}=26$, $B_{sat}=1{,}65\,\mathrm T$ und $L(0)=584\,\mu\mathrm H$.

![Abbildung 3: B(H)-Kennlinie](Bilder/abbildung_03_bh_kennlinie.svg)

![Abbildung 4: Induktivitätskennlinien](Bilder/abbildung_04_induktivitaet.svg)

![Abbildung 5: Permeabilitätskennlinien](Bilder/abbildung_05_permeabilitaet.svg)

# 8 Stromwelligkeit und Flussdichtehub

$$\Delta I_{pp}=\frac{V_L}{L_{diff}f_s},\qquad \Delta B_{pp}=\frac{V_L}{NA_ef_s}.$$

![Abbildung 7: Stromwelligkeit](Bilder/abbildung_07_stromwelligkeit.svg)

![Abbildung 8: Flussdichtehub](Bilder/abbildung_08_flussdichtehub.svg)

Im vereinfachten PWM-Modell ergeben sich maximal rund 5,05 A Stromwelligkeit und 98 mT Flussdichtehub. Die endgültige Berechnung muss mit der realen SVM erfolgen.

# 9 Elektrische Wicklungsverluste

Für die Vorabschätzung wurde eine mittlere Windungslänge von etwa 105 mm angesetzt. Bei 80 Windungen ergibt sich eine Kupferlänge von etwa 8,4 m und damit näherungsweise

$$R_{DC,25}\approx51{,}0\,\mathrm{m\Omega},\qquad R_{DC,120}\approx70{,}0\,\mathrm{m\Omega}.$$

![Abbildung 6: DC-Kupferverluste](Bilder/abbildung_06_kupferverluste.svg)

# 10 Kernverluste

Für Magnetics High Flux 26 µ sind noch validierte Steinmetzparameter einzutragen. Die Parameter des 60-µ-Materials aus Beispiel 2 dürfen nicht übernommen werden.

# 11 Gesamtverluste

$$P_{ges}=P_{Cu,DC}+P_{Cu,HF}+P_{Kern}.$$

![Abbildung 9: Vorläufige Gesamtverluste](Bilder/abbildung_09_gesamtverluste.svg)

Der Kernverlustanteil ist in dieser Kurve nur ein gekennzeichneter Platzhalter. Die Abbildung ist nicht zur Freigabe geeignet.

# 12 Thermik

$$\Delta T=P_{ges}R_{th}.$$

![Abbildung 10: Temperaturabschätzung](Bilder/abbildung_10_temperaturabschaetzung.svg)

Die Vorabschätzung verwendet beispielhaft $R_{th}=0{,}65\,\mathrm{K/W}$. Eine thermische Freigabe erfordert Messungen bei 20 kW Dauerbetrieb und 40 kW für 0,5 s.

# 13 Fertigung

Kerne prüfen, axial ausrichten und isolierend verkleben; Kanten schützen; Rechteckleiter mit kontrolliertem Biegeradius wickeln; Anschlüsse separat zugentlasten. Der einlagige Aufbau ist durch einen maßstäblichen Wickelversuch zu bestätigen.

# 14 Prüfung

Erforderlich sind Sichtprüfung, Windungsprüfung, Gleichstromwiderstand, Anfangsinduktivität, L(I)-Kennlinie, Isolation, Verlust- und Temperaturmessung sowie Überlastprüfung.

# 15 PLECS-/MATLAB-Parametersatz

```matlab
D_a_mm = 50.80;
D_i_mm = 31.75;
h_mm = 3*13.46;
N_Wdg = 80;
N_Litze = 1;
d_Litze_mm = sqrt(4*0.72*4/pi);
A_mag = 3*125e-6;
l_mag_mm = 127;
mu_r_0 = 26;
mu_r_sat = 1;
B_sat = 1.65;
U_LL_rms = 400;
U_DC = 750;
f_sw = 70e3;
L_target = 584e-6;
R_DC_25_est = 51.0e-3;
R_DC_120_est = 70.0e-3;
```

# 16 Bewertung

Die Geometrie, die einlagige Rechteckleiterdarstellung und die Diagramme sind konsistent zum aktuellen Parametersatz neu aufgebaut. Offen bleiben der Messabgleich von 584 µH, die reale mittlere Windungslänge, HF-Kupferverluste, validierte Kernverlustparameter und die thermische Freigabe.

# 17 Quellen

- Magnetics-Datenblatt und Materialdaten für 0058717A2 / High Flux 26 µ.
- [Formelsammlung](../../../Formelsammlung/README.md).
- [Referenzbeispiel 2](../2_PFC_20kW_525uH_3x56mm_HighFlux_23mOhm/README.md).
