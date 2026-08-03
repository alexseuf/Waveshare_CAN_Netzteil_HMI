---
title: "PFC-Drossel 584 µH – 3 × 50,8 mm High Flux – Gesamtausgabe"
version: "Revision 1.0"
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

---

# 1 Zweck und Geltungsbereich

Dieses Beispiel dokumentiert den Entwurfsstand einer dreiphasigen PFC-Drossel mit einer Anfangsinduktivität von etwa 584 µH. Der Aufbau besteht aus drei axial gestapelten Magnetics-0058717A2-High-Flux-Ringkernen und einer Wicklung aus 80 Windungen Rechteckkupferleiter mit 0,72 mm × 4,00 mm Leiterquerschnitt. Die endgültige Freigabe setzt Messungen am realen Muster voraus.

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
| Zielinduktivität | ca. 584 µH |

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
| Kernvolumen | 47,63 cm³ |
| Modellwert $B_{sat}$ | 1,65 T |

# 4 Wicklungsaufbau

| Parameter | Wert |
|---|---:|
| Windungszahl | 80 |
| Leiter | Rechteckkupfer 0,72 × 4,00 mm |
| Kupferquerschnitt | 2,88 mm² |
| Ersatz-Runddurchmesser | 1,915 mm |

$$d_{eq}=\sqrt{\frac{4\cdot0{,}72\cdot4}{\pi}}\approx1{,}915\,\mathrm{mm}.$$

Der Ersatzdurchmesser erhält den Gleichstromquerschnitt, bildet die HF-Stromverteilung des Rechteckleiters aber nicht geometrisch korrekt ab.

# 5 Mechanischer Aufbau

Der dreifache Kernstapel wird fluchtend und elektrisch isolierend verklebt. Kernkanten sind zusätzlich zu schützen. Rechteckleiter dürfen weder scharf geknickt noch lokal eingeschnürt werden. Mindestbiegeradius, Isolation und Lagenaufbau sind durch ein Wickelmuster nachzuweisen.

# 6 Berechnungsgrundlagen

$$L_0=\frac{\mu_0\mu_{r0}N^2A_e}{l_e}.$$

Mit $\mu_{r0}=26$, $N=80$, $A_e=375\,\mathrm{mm^2}$ und $l_e=127\,\mathrm{mm}$ ergibt sich aus den gerundeten Parametern etwa 617 µH. Dieser Idealwert liegt über dem Projektziel von 584 µH. Für Spezifikation und Simulation ist daher die Herstellerkennlinie beziehungsweise der gemessene Anfangswert maßgebend.

$$H[\mathrm{Oe}]=\frac{4\pi NI}{l_e[\mathrm{mm}]}$$

$$\Delta B_{pp}=\frac{V_{DC}d(1-d)}{NA_ef_s}.$$

# 7 Magnetische Kennlinien

Für den Ursprung wird der projektspezifische Wert

$$L_{diff}(0)=L_{sec}(0)=584\,\mu\mathrm{H}$$

angesetzt. Die stromabhängige Kennlinie ist mit dem Herstellerfit für High Flux 26 µ oder Messdaten des dreifachen Kernstapels zu erzeugen.

# 8 Stromwelligkeit und Flussdichtehub

$$\Delta I_{pp}=\frac{V_L}{L_{diff}f_s},\qquad
\Delta B_{pp}=\frac{V_L}{NA_ef_s}.$$

Der Maximalwert ist über die vollständige Netzperiode und die tatsächlich verwendete SVM zu bestimmen.

# 9 Elektrische Wicklungsverluste

$$R_{DC,25}=\rho_{Cu,25}\frac{l_{Cu}}{A_{Cu}},\qquad A_{Cu}=2{,}88\,\mathrm{mm^2}.$$

Da mittlere Windungslänge und endgültiger Lagenplan noch offen sind, wird kein unbelegter Widerstandswert angegeben. HF-Zusatzverluste des 0,72-mm-Rechteckleiters sind separat zu bestimmen.

# 10 Kernverluste

$$p_v=aB^bf^c.$$

Es sind ausschließlich validierte Koeffizienten für Magnetics High Flux 26 µ zu verwenden. Die 60-µ-Parameter aus Beispiel 2 sind nicht übertragbar.

# 11 Gesamtverluste

$$P_{ges}=P_{Cu,DC}+P_{Cu,HF}+P_{Kern}.$$

Belastbare Zahlenwerte erfordern den endgültigen Lagenplan, die mittlere Windungslänge, validierte Kernverlustparameter und den simulierten Flussdichteverlauf.

# 12 Thermik

$$\Delta T=P_{ges}R_{th}.$$

Die Freigabe erfordert Temperaturmessungen bei 20 kW Dauerbetrieb sowie einen 40-kW-Überlastversuch über 0,5 s.

# 13 Fertigung

Kerne prüfen, axial ausrichten und isolierend verkleben; Kanten schützen; Rechteckleiter mit kontrolliertem Biegeradius wickeln; Lagen isolieren und fixieren; Anschlüsse separat zugentlasten. Der endgültige Lagenplan ist durch einen maßstäblichen Wickelversuch zu dokumentieren.

# 14 Prüfung

Erforderlich sind Sichtprüfung, Windungsprüfung, Gleichstromwiderstand, Anfangsinduktivität, L(I)-Kennlinie, Isolation, Verlust- und Temperaturmessung bei Dauerlast sowie Überlastprüfung. Als Zielwert gelten 584 µH; die zulässige Toleranz ist noch festzulegen.

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

A_Cu_mm2 = 0.72*4;
d_eq_mm = sqrt(4*A_Cu_mm2/pi);
U_LL_rms = 400;
U_DC = 750;
f_sw = 70e3;
L_target = 584e-6;
```

# 16 Bewertung

Offen sind der Messabgleich des Zielwerts 584 µH, der realisierbare Lagenplan, die mittlere Windungslänge und der DC-Widerstand, validierte 26-µ-Kernverlustparameter sowie thermische und isolationstechnische Prüfungen. Bis dahin ist dies ein Entwurfsstand, keine Fertigungsfreigabe.

# 17 Quellen

- Magnetics-Datenblatt und Materialdaten für 0058717A2 / High Flux 26 µ.
- [Formelsammlung](../../../Formelsammlung/README.md).
- [Referenzbeispiel 2](../2_PFC_20kW_525uH_3x56mm_HighFlux_23mOhm/README.md).
