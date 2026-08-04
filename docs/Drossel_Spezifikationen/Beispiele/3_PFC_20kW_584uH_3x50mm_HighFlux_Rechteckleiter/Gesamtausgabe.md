---
title: "PFC-Drossel 584 µH – 3 × 50,8 mm High Flux – Gesamtausgabe"
version: "Revision 1.2"
last_updated: "2026-08-04"
source: "Automatisch aus den Einzelkapiteln 01 bis 17 zusammengestellt"
---

# PFC-Drossel 584 µH – 3 × Magnetics 0058717A2

**High Flux 26 µ · 80 Windungen · Rechteckleiter 0,72 × 4,00 mm · Kupferquerschnitt 2,88 mm²**

Diese Gesamtausgabe wurde aus den aktuellen Einzelkapiteln des Beispielprojekts zusammengestellt. Die Abbildungen werden über relative Pfade aus dem Unterordner `Bilder` eingebunden.

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

Dieses Beispiel dokumentiert den Entwurfsstand einer dreiphasigen PFC-Drossel mit einer Anfangsinduktivität von etwa 584 µH. Der Aufbau besteht aus drei axial gestapelten Magnetics-0058717A2-High-Flux-Ringkernen und einer Wicklung aus 80 Windungen Rechteckkupferleiter mit 0,72 mm × 4,00 mm Leiterquerschnitt.

Die Spezifikation umfasst magnetische, elektrische, thermische und fertigungstechnische Berechnungsgrundlagen. Die endgültige Freigabe setzt Messungen am realen Muster voraus.

---

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
| Zielinduktivität bei kleinem Signal | ca. 584 µH |

Der Dauerbetrieb ist für die thermische Auslegung maßgebend. Die Kurzzeitüberlast ist hinsichtlich Stromspitze, Verlustenergie, Temperaturhub und bleibender Änderungen zu prüfen.

---

# 3 Magnetischer Aufbau

| Parameter | Wert |
|---|---:|
| Kern | 3 × Magnetics 0058717A2 |
| Material | High Flux, µ = 26 |
| Außendurchmesser | 50,80 mm |
| Innendurchmesser | 31,75 mm |
| Höhe je Kern | 13,46 mm |
| Gesamthöhe | 40,38 mm |
| Effektiver Querschnitt $A_e$ | 375 mm² |
| Effektive Weglänge $l_e$ | 127 mm |
| Kernvolumen $V_e=A_e l_e$ | 47,63 cm³ |
| Modellwert $B_{sat}$ | 1,65 T |

Die drei Ringkerne werden axial und fluchtend gestapelt. Bestellnummer, Beschichtungstoleranzen und effektive Magnetdaten sind vor der Fertigungsfreigabe mit dem aktuellen Herstellerdatenblatt abzugleichen.

---

# 4 Wicklungsaufbau

| Parameter | Wert |
|---|---:|
| Windungszahl | 80 |
| Leiterart | Rechteckkupferleiter |
| Leiterabmessung | 0,72 mm × 4,00 mm |
| Wicklungsaufbau | einlagig |
| Leiterorientierung | 4,00 mm axial, 0,72 mm radial |
| Kupferquerschnitt | 2,88 mm² |
| Ersatz-Runddurchmesser | 1,915 mm |
| PLECS-Ersatz | $N_{Litze}=1$, $d_{Litze}=\sqrt{4A/\pi}$ |

Der Rechteckleiter besitzt den Kupferquerschnitt

$$A_{Cu}=0{,}72\,\mathrm{mm}\cdot4{,}00\,\mathrm{mm}=2{,}88\,\mathrm{mm^2}.$$

Die Wicklung wird in der dargestellten Vorzugsvariante einlagig ausgeführt. Die Leiterbreite von 4,00 mm liegt axial über der Höhe des dreifachen Kernstapels; die Leiterdicke von 0,72 mm wirkt radial. Die Darstellung zeigt 80 gleichmäßig über den Umfang verteilte Windungen.

![Abbildung 14: Einlagiger Wicklungsaufbau mit Rechteckleiter](Bilder/abbildung_14_wicklungsaufbau_rechteckleiter_einlagig.svg)

*Abbildung 14: Schematische Draufsicht und axialer Querschnitt des einlagigen Wicklungsaufbaus mit 80 Windungen Rechteckleiter 0,72 mm × 4,00 mm auf drei gestapelten Magnetics-0058717A2-Ringkernen.*

Für Modelle, die nur einen Runddrahtdurchmesser akzeptieren, wird ein flächengleicher Ersatzdurchmesser verwendet:

$$d_{eq}=\sqrt{\frac{4A_{Cu}}{\pi}}=\sqrt{\frac{4\cdot0{,}72\cdot4}{\pi}}\approx1{,}915\,\mathrm{mm}.$$

Dieser Ersatzdurchmesser erhält den Gleichstromquerschnitt, bildet Skin- und Proximity-Effekt des Rechteckleiters jedoch nicht geometrisch korrekt ab. Für die Wicklungsfertigung sind Isolationsdicke, Biegeradius, Mindestabstand zwischen benachbarten Windungen und die tatsächlich nutzbare Fenstergeometrie separat zu verifizieren.

---

# 5 Mechanischer Aufbau

Der dreifache Kernstapel ist fluchtend auszurichten und mit elektrisch isolierendem, temperaturbeständigem Klebstoff zu verbinden. Zwischen Kernbeschichtung und Rechteckleiter ist ein geeigneter Kanten-, Abrieb- und Durchschlagschutz vorzusehen.

![Abbildung 1: Mechanischer Aufbau](Bilder/abbildung_01_mechanischer_aufbau.svg)

*Abbildung 1: Neu berechnete schematische Seiten- und Draufsicht des dreifachen Kernstapels mit einlagigem Rechteckleiter.*

Der Rechteckleiter darf beim Umformen weder scharf geknickt noch an den Kernkanten plastisch eingeschnürt werden. Anschlussbereiche sind separat mechanisch zu entlasten; die Drossel darf nicht über ihre elektrischen Anschlüsse befestigt werden.

Vor der Fertigung sind der Mindestbiegeradius, die tatsächliche Lack- oder Folienisolation und der Lagenaufbau anhand eines Wickelmusters zu bestätigen.

---

# 6 Berechnungsgrundlagen

## 6.1 Anfangsinduktivität

Die Anfangsinduktivität wird aus der magnetischen Reluktanz berechnet:

$$L_0=\frac{\mu_0\mu_{r0}N^2A_e}{l_e}.$$

Mit

- $\mu_0=4\pi\cdot10^{-7}\,\mathrm{H/m}$,
- $\mu_{r0}=26$,
- $N=80$,
- $A_e=375\,\mathrm{mm^2}=375\cdot10^{-6}\,\mathrm{m^2}$,
- $l_e=127\,\mathrm{mm}=0{,}127\,\mathrm{m}$

folgt

$$L_0=\frac{4\pi\cdot10^{-7}\cdot26\cdot80^2\cdot375\cdot10^{-6}}{0{,}127}
\approx617\,\mu\mathrm{H}.$$

Der aus den gerundeten Geometrie- und Anfangspermeabilitätswerten berechnete Idealwert liegt damit oberhalb des Projektzielwerts von 584 µH. Für Simulation und Spezifikation ist deshalb die Herstellerkennlinie beziehungsweise der gemessene Anfangswert maßgebend. Die Abweichung ist vor Freigabe am realen Kernstapel zu verifizieren.

## 6.2 Feldstärke

$$H[\mathrm{Oe}]=\frac{4\pi NI}{l_e[\mathrm{mm}]}$$

mit $N=80$ und $l_e=127\,\mathrm{mm}$.

## 6.3 Flussdichtehub

Für das vereinfachte phasenbezogene Zweilevel-PWM-Modell gilt

$$d=0{,}5+\frac{v_{Phase}}{V_{DC}}$$

und

$$\Delta B_{pp}=\frac{V_{DC}d(1-d)}{NA_ef_s}.$$

SVM-Gleichtaktanteile, Totzeiten und diskrete Schaltzustände sind in dieser Näherung nicht enthalten.

---

# 7 Magnetische Kennlinien

Die differentielle Induktivität ist für die Stromwelligkeit um den momentanen Arbeitspunkt maßgebend. Die Sekanteninduktivität beschreibt die Flussverkettung bezogen auf den Strom.

Für den Ursprung wird der projektspezifische Anfangswert angesetzt:

$$L_{diff}(0)=L_{sec}(0)=584\,\mu\mathrm{H}.$$

Die neu berechneten Kurven verwenden als glatte Entwurfsnäherung

$$B(H)=B_{sat}\tanh\left(\frac{\mu_0\mu_{r0}H}{B_{sat}}\right)$$

mit $\mu_{r0}=26$ und $B_{sat}=1{,}65\,\mathrm T$. Daraus werden differentielle und Sekantenwerte konsistent abgeleitet.

![Abbildung 3: B(H)-Kennlinie](Bilder/abbildung_03_bh_kennlinie.svg)

*Abbildung 3: Berechnete B(H)-Entwurfskennlinie für High Flux 26 µ mit markierten Arbeitspunkten.*

![Abbildung 4: Differentielle und Sekanteninduktivität](Bilder/abbildung_04_induktivitaet.svg)

*Abbildung 4: Differentielle und Sekanteninduktivität über dem Strom, ausgehend von 584 µH bei 0 A.*

![Abbildung 5: Differentielle und Sekantenpermeabilität](Bilder/abbildung_05_permeabilitaet.svg)

*Abbildung 5: Differentielle und Sekantenpermeabilität der verwendeten Entwurfsnäherung.*

Die Diagramme sind reproduzierbare Auslegungsnäherungen. Vor einer Fertigungsfreigabe sind sie durch Herstellerdaten oder eine gemessene $L(I)$- beziehungsweise B(H)-Kennlinie des dreifachen Kernstapels zu ersetzen oder zu bestätigen.

---

# 8 Stromwelligkeit und Flussdichtehub

Für eine lokale Schaltperiode wird die Stromwelligkeit näherungsweise mit

$$\Delta I_{pp}=\frac{V_L}{L_{diff}f_s}$$

berechnet. Wegen der stromabhängigen Permeabilität ist dabei die differentielle Induktivität am jeweiligen Arbeitspunkt einzusetzen.

Der Flussdichtehub folgt aus

$$\Delta B_{pp}=\frac{V_L}{NA_ef_s}.$$

Die folgenden Kurven wurden über eine vollständige Netzperiode mit dem vereinfachten phasenbezogenen PWM-Modell, $U_{DC}=750\,\mathrm V$, $f_s=70\,\mathrm{kHz}$, $N=80$ und $A_e=375\,\mathrm{mm^2}$ neu berechnet.

![Abbildung 7: Stromwelligkeit](Bilder/abbildung_07_stromwelligkeit.svg)

*Abbildung 7: Berechnete Stromwelligkeit über einer Netzperiode im vereinfachten PWM-Modell.*

![Abbildung 8: Flussdichtehub](Bilder/abbildung_08_flussdichtehub.svg)

*Abbildung 8: Berechneter hochfrequenter Flussdichtehub über einer Netzperiode.*

Die dargestellte maximale Stromwelligkeit liegt in dieser Näherung bei rund $5{,}05\,\mathrm A_{pp}$, der maximale Flussdichtehub bei rund $98\,\mathrm{mT}_{pp}$. SVM-Gleichtaktanteile, Totzeiten und diskrete Schaltzustände sind in dieser Darstellung noch nicht enthalten.

---

# 9 Elektrische Wicklungsverluste

## 9.1 Gleichstromwiderstand

Der Gleichstromwiderstand wird aus der tatsächlichen mittleren Windungslänge $l_{Cu}$ und dem Kupferquerschnitt bestimmt:

$$R_{DC,25}=\rho_{Cu,25}\frac{l_{Cu}}{A_{Cu}},$$

mit $A_{Cu}=2{,}88\,\mathrm{mm^2}$. Für die vorläufige Diagrammberechnung wurde aus dem dreifachen Kernstapel eine mittlere Windungslänge von etwa $105\,\mathrm{mm}$ und damit eine gesamte Kupferlänge von etwa $8{,}4\,\mathrm m$ angesetzt. Daraus folgt näherungsweise

$$R_{DC,25}\approx51{,}0\,\mathrm{m\Omega},\qquad R_{DC,120}\approx70{,}0\,\mathrm{m\Omega}.$$

![Abbildung 6: DC-Kupferverluste](Bilder/abbildung_06_kupferverluste.svg)

*Abbildung 6: Vorläufig berechnete DC-Kupferverluste bei 25 °C und 120 °C.*

Die Werte sind eine geometrische Vorabschätzung und müssen am Wickelmuster gemessen werden. Die hochfrequenten Zusatzverluste des 0,72 mm dicken Rechteckleiters sind wegen Skin- und Proximity-Effekt separat zu bestimmen; der flächengleiche Runddrahtdurchmesser im PLECS-Parametersatz ist dafür nur eine Näherung.

---

# 10 Kernverluste

Die Kernverluste werden mit den für Magnetics High Flux 26 µ gültigen Herstellerparametern und dem tatsächlich auftretenden Flussdichtehub berechnet. Eine allgemeine Steinmetzform lautet

$$p_v=a\,B^b\,f^c.$$

Einheiten und Koeffizienten sind gemeinsam aus derselben Datenquelle zu übernehmen. Die für High Flux 60 µ im zweiten Beispiel verwendeten Parameter dürfen nicht ungeprüft auf das 26-µ-Material übertragen werden.

Das Gesamtverlustmodell muss mindestens den 70-kHz-Flussdichtehub über die Netzperiode integrieren. Bei nichtsinusförmiger Anregung ist eine geeignete erweiterte Steinmetz-Methode oder die direkte Herstellerkennlinie zu verwenden.

---

# 11 Gesamtverluste

Die Gesamtverluste ergeben sich aus

$$P_{ges}=P_{Cu,DC}+P_{Cu,HF}+P_{Kern}.$$

Für die neu berechnete Vorabschätzung wurden die DC-Kupferverluste aus $R_{DC,25}=51{,}0\,\mathrm{m\Omega}$ beziehungsweise $R_{DC,120}=70{,}0\,\mathrm{m\Omega}$ verwendet. Da für High Flux 26 µ noch keine validierten Steinmetzparameter hinterlegt sind, enthält die Kurve nur einen ausdrücklich als Platzhalter gekennzeichneten Kernverlustanteil.

![Abbildung 9: Vorläufige Gesamtverluste](Bilder/abbildung_09_gesamtverluste.svg)

*Abbildung 9: Vorläufige Gesamtverlustkurve mit gekennzeichnetem Platzhalter für die Kernverluste.*

Die Abbildung dient ausschließlich zum Vergleich der Größenordnung. Für die Freigabe müssen $P_{Cu,HF}$ und $P_{Kern}$ aus der realen Strom- und Flussdichtewellenform sowie validierten Materialdaten neu berechnet werden.

---

# 12 Thermik

Die thermische Auslegung basiert auf den Gesamtverlusten aus Kapitel 11 und dem thermischen Widerstand vom Kupfer beziehungsweise Kern zur Umgebung.

Für eine erste stationäre Abschätzung gilt

$$\Delta T=P_{ges}R_{th}.$$

Die neu berechnete Vorabschätzung verwendet beispielhaft $R_{th}=0{,}65\,\mathrm{K/W}$ und die vorläufigen Verlustkurven aus Kapitel 11.

![Abbildung 10: Temperaturabschätzung](Bilder/abbildung_10_temperaturabschaetzung.svg)

*Abbildung 10: Stationäre Temperaturabschätzung auf Basis der vorläufigen Gesamtverluste.*

Die dargestellten Werte sind keine thermische Freigabe. Wegen des dreifachen Kernstapels und der 80 Windungen ist die Luftführung durch die Kernöffnung und entlang der Wicklungsoberfläche besonders zu beachten. Die Freigabe erfordert Temperaturmessungen bei 20 kW Dauerbetrieb sowie einen 40-kW-Überlastversuch über 0,5 s; Wicklungs-, Kern- und Umgebungstemperatur sind getrennt zu erfassen.

---

# 13 Fertigung

1. Drei Kerne 0058717A2 auf Beschädigung, Abmessungen und Beschichtung prüfen.
2. Kernstapel axial ausrichten und isolierend verkleben.
3. Zusätzlichen Kanten- und Abriebschutz aufbringen.
4. Rechteckleiter mit kontrolliertem Biegeradius wickeln; Leiterverdrehungen vermeiden.
5. Lagen gegeneinander isolieren und gegen Verschieben sichern.
6. Wicklungsanfang und Wicklungsende separat zugentlasten.
7. Anschlussflächen erst nach dem Wickeln abisolieren und für die vorgesehene Verbindungstechnik vorbereiten.
8. Drossel imprägnieren oder mechanisch fixieren, ohne Kühlkanäle vollständig zu verschließen.

Der endgültige Lagenplan ist vor Serienfertigung in einem maßstäblichen Wickelversuch zu dokumentieren.

---

# 14 Prüfung

Vor der elektrischen Freigabe sind mindestens folgende Prüfungen durchzuführen:

- Sichtprüfung von Kernbeschichtung, Isolationslagen und Leiterbiegungen,
- Windungszahl- und Anschlussprüfung,
- Gleichstromwiderstand bei dokumentierter Temperatur,
- Anfangsinduktivität bei kleinem Prüfstrom,
- Induktivitätskennlinie über dem Gleichstrom-Arbeitspunkt,
- Isolationswiderstand und Spannungsfestigkeit gemäß Systemanforderung,
- Verlust- und Temperaturmessung bei 20 kW Dauerbetrieb,
- Überlastprüfung bei 40 kW für 0,5 s,
- Kontrolle auf bleibende Induktivitäts-, Widerstands- oder Isolationsänderungen nach dem Überlastversuch.

Als Zielwert für die Anfangsinduktivität werden 584 µH verwendet. Die zulässige Toleranz ist vor der Freigabe festzulegen.

---

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

% Geometrischer Leiterquerschnitt und flächengleicher Runddurchmesser
A_Cu_mm2 = 0.72*4;
d_eq_mm = sqrt(4*A_Cu_mm2/pi);

U_LL_rms = 400;
U_DC = 750;
f_sw = 70e3;
L_target = 584e-6;
```

Der Parameter `d_Litze_mm` ist ein flächengleicher Ersatzdurchmesser für den Rechteckleiter. Er bildet den Gleichstromquerschnitt ab, nicht jedoch die tatsächliche HF-Stromverteilung.

Für Kernverlustberechnungen sind die zum Magnetics-High-Flux-26-µ-Material gehörenden Steinmetzparameter einzusetzen. Parameter anderer Permeabilitätsvarianten dürfen nicht ungeprüft übernommen werden.

---

# 16 Bewertung

Der Aufbau erreicht mit drei kompakten 0058717A2-Kernen und 80 Windungen eine hohe Anfangsinduktivität bei kleiner Kernbaugröße. Der Rechteckleiter mit 2,88 mm² Kupferquerschnitt ermöglicht eine definierte mechanische Wicklung, verlangt bei 70 kHz jedoch eine sorgfältige Bewertung der hochfrequenten Zusatzverluste.

Offene Freigabepunkte sind:

- Abgleich des Zielwerts 584 µH mit dem aus gerundeten Geometriedaten berechneten Idealwert,
- Festlegung und Nachweis des realisierbaren Lagenplans,
- Ermittlung der mittleren Windungslänge und des Gleichstromwiderstands,
- Verwendung verifizierter High-Flux-26-µ-Kernverlustparameter,
- thermische Messung bei Dauer- und Kurzzeitlast,
- Prüfung der Isolations- und Biegereserven des Rechteckleiters.

Bis zum Abschluss dieser Punkte bleibt die Spezifikation ein belastbarer Entwurfsstand, jedoch keine Fertigungsfreigabe.

---

# 17 Quellen

- Magnetics: Datenblatt und Materialdaten zum Ringkern 0058717A2, High Flux 26 µ.
- Projektdokumentation: [Formelsammlung](../../../Formelsammlung/README.md).
- Referenzaufbau: [Beispiel 2 – PFC-Drossel 525 µH](../2_PFC_20kW_525uH_3x56mm_HighFlux_23mOhm/README.md).

Vor der Freigabe sind Bestellnummer, Abmessungen, Permeabilität, Sättigungsflussdichte und Kernverlustkoeffizienten gegen den aktuellen Herstellerstand zu prüfen.
