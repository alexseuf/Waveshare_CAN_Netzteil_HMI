---
title: "Formelsammlung – Gesamtausgabe"
version: "automatisch erzeugt"
---

# Formelsammlung – Gesamtausgabe

Diese Datei enthält alle Kapitel in einer fortlaufenden Leseansicht. Sie wird automatisch aus den einzelnen Kapiteldateien erzeugt.

## Inhaltsverzeichnis

### Band B – Wicklungsberechnung

- [B1.1 Leiterarten und Grundlagen](#b11-leiterarten-und-grundlagen)
- [B1.2 Skin- und Proximity-Effekt](#b12-skin--und-proximity-effekt)
- [B1.3 Litzenaufbau und Packungsfaktor](#b13-litzenaufbau-und-packungsfaktor)
- [B1.4 Stromdichte und Auswahlkriterien](#b14-stromdichte-und-auswahlkriterien)
- [B1.5 Praxisleitfaden Leiterauswahl](#b15-praxisleitfaden-leiterauswahl)

### Band C – Magnetische Auslegung

- [C1 Geometrische Kenngrößen](#c1-geometrische-kenngrößen)
- [C2 Der magnetische Kreis](#c2-der-magnetische-kreis)
- [C3 Induktivitätsberechnung](#c3-induktivitätsberechnung)
- [C4 Flussdichte und Sättigung](#c4-flussdichte-und-sättigung)
- [C5 Magnetische Kernmaterialien](#c5-magnetische-kernmaterialien)
- [C6 Kernverluste](#c6-kernverluste)
- [C7 Thermische Auslegung](#c7-thermische-auslegung)
- [C8 Vollständige Auslegungsbeispiele](#c8-vollständige-auslegungsbeispiele)

---

# Band B – Wicklungsberechnung


---

## B1.1 Leiterarten und Grundlagen

## Grundlagen der Litzenberechnung

### Kupferquerschnitt eines Einzeldrahtes

```math
A_{\mathrm{einz}}=\frac{\pi d^2}{4}
```

mit

- $d$ = Kupferdurchmesser eines Einzeldrahtes

### Gesamtkupferquerschnitt einer Litze

```math
A_{\mathrm{Cu}}=n\cdot A_{\mathrm{einz}}
```

mit

- $n$ = Anzahl der Einzeldrähte
- $A_{\mathrm{einz}}$ = Kupferquerschnitt eines Einzeldrahtes

<!-- Gesamtausgabe-Workflow auslösen -->

---

## B1.2 Skin- und Proximity-Effekt

### Ziel

Dieses Kapitel erläutert die Ursachen frequenzabhängiger Kupferverluste und gibt Auslegungshinweise für die Wahl des Einzeldrahtdurchmessers einer HF-Litze.

### Skin-Effekt

Bei Wechselstrom konzentriert sich der Strom mit steigender Frequenz zunehmend an der Leiteroberfläche. Dadurch verkleinert sich der effektiv genutzte Leiterquerschnitt und der Wechselstromwiderstand steigt an.

Skintiefe:

```math
\delta=\sqrt{\frac{2\rho}{\omega\mu}}
```

mit ρ = spezifischer Widerstand, ω = 2πf und μ ≈ μ₀ für Kupfer.

| Frequenz | Skintiefe δ [mm] |
|---:|---:|
| 20 kHz | 0,467 |
| 50 kHz | 0,296 |
| 70 kHz | 0,250 |
| 100 kHz | 0,209 |
| 200 kHz | 0,148 |

### Auswahl des Einzeldrahtdurchmessers

Als Faustregel sollte der Kupferdurchmesser eines Einzeldrahtes höchstens etwa das Zweifache der Skintiefe betragen. Bei einer Schaltfrequenz von 70 kHz liegt die Skintiefe in Kupfer bei rund 0,25 mm. Einzeldrähte mit 0,10 mm Durchmesser sind daher für diesen Frequenzbereich gut geeignet.

### Proximity-Effekt

Neben dem Skin-Effekt verursachen Magnetfelder benachbarter Leiter eine ungleiche Stromverteilung im Leiterquerschnitt. Dieser Proximity-Effekt erhöht den AC-Widerstand zusätzlich und ist insbesondere bei dicht gepackten Wicklungen relevant.

### Maßnahmen zur Verringerung der AC-Verluste

- Verwendung von HF-Litze mit kleinem Einzeldrahtdurchmesser
- gleichmäßige Verteilung der Litze über den Wickelraum
- Vermeidung unnötig vieler Wicklungslagen
- ausreichender Abstand zu stark streuenden Luftspalten

### Beispiel für 70-kHz-PFC-Drosseln

Für PFC-Drosseln im Bereich 50…100 kHz haben sich Litzen mit 0,071…0,10 mm Einzeldrahtdurchmesser bewährt. Die endgültige Auswahl erfolgt unter Berücksichtigung von Strombelastbarkeit, Wickelraum und Fertigung.

---

## B1.3 Litzenaufbau und Packungsfaktor

### Ziel

Dieses Kapitel beschreibt den geometrischen Aufbau von HF-Litzen sowie die Berechnung des Kupferquerschnitts, des Packungsfaktors und des benötigten Wickelraums.

### Aufbau einer HF-Litze

Eine HF-Litze besteht aus vielen einzeln lackisolierten Kupferdrähten, die in mehreren Verseilstufen miteinander verflochten werden. Ziel ist eine möglichst gleichmäßige Stromverteilung und geringe AC-Verluste.

### Packungsarten

| Packungsart | typischer Packungsfaktor | Bemerkung |
|---|---:|---|
| quadratisch | 0,78 | einfach herstellbar |
| hexagonal | 0,907 (ideal) | theoretisch dichteste Kreispackung |
| reale HF-Litze | 0,60…0,75 | abhängig von Verseilung und Isolation |

### Berechnung des Kupferquerschnitts

```math
A_{\mathrm{Cu}}=n\cdot\frac{\pi d^2}{4}
```

mit:

- $n$ = Anzahl der Einzeldrähte
- $d$ = Kupferdurchmesser eines Einzeldrahtes

### Abschätzung des Litzenaußendurchmessers

Für eine erste mechanische Abschätzung kann der Außendurchmesser aus dem Gesamtkupferquerschnitt und einem Packungsfaktor $\eta$ abgeschätzt werden:

```math
A_{\mathrm{gesamt}}=\frac{A_{\mathrm{Cu}}}{\eta}
```

```math
d_{\mathrm{außen}}\approx\sqrt{\frac{4A_{\mathrm{gesamt}}}{\pi}}
```

Herstellerangaben sind für die endgültige Konstruktion zu bevorzugen.

### Beispielrechnungen

| Litze | $A_{\mathrm{Cu}}$ [mm²] | $\eta$ | $A_{\mathrm{gesamt}}$ [mm²] | $d_{\mathrm{außen}}$ ca. [mm] |
|---|---:|---:|---:|---:|
| 160 × 0,10 | 1,26 | 0,68 | 1,85 | 1,53 |
| 210 × 0,10 | 1,65 | 0,68 | 2,43 | 1,76 |
| 630 × 0,10 | 4,95 | 0,68 | 7,28 | 3,04 |
| 680 × 0,10 | 5,34 | 0,68 | 7,85 | 3,16 |
| 800 × 0,10 | 6,28 | 0,68 | 9,24 | 3,43 |

### Hinweise für die Wicklungsberechnung

- Der mechanische Außendurchmesser hängt zusätzlich von Lackdicke, Verseilung und Fertigungstoleranzen ab.
- Für die Auslegung des Wickelraums sollte immer der Herstellerwert des Litzenaußendurchmessers verwendet werden.
- Die berechneten Werte dienen als erste Abschätzung in der Konzeptphase.

---

## B1.4 Stromdichte und Auswahlkriterien

### Ziel

Dieses Kapitel beschreibt die Auswahl eines geeigneten Leiters bzw. einer HF-Litze anhand von Strombelastbarkeit, Kühlbedingungen, Schaltfrequenz und verfügbarem Wickelraum.

### Stromdichte

```math
J=\frac{I_{\mathrm{RMS}}}{A_{\mathrm{Cu}}}
```

mit:

- $J$ in A/mm²
- $I_{\mathrm{RMS}}$ als Effektivstrom
- $A_{\mathrm{Cu}}$ als effektiver Kupferquerschnitt

### Richtwerte

| Kühlung | empfohlene Stromdichte [A/mm²] | Bemerkung |
|---|---:|---|
| natürliche Konvektion | 2…4 | geringer Temperaturanstieg |
| forcierte Luftkühlung | 4…8 | typisch für PFC-Drosseln |
| Flüssigkeitskühlung | 6…12 | projektabhängig nach Thermik |
| Kurzzeitbetrieb | 8…15 | nur transiente Belastung |

### Auswahlkriterien

- Die Schaltfrequenz bestimmt den zulässigen Einzeldrahtdurchmesser.
- Der verfügbare Wickelraum begrenzt Litzenquerschnitt und Windungszahl.
- Die zulässige Wicklungstemperatur bestimmt die maximale Dauerstromdichte.
- Mechanische Biegeradien und Fertigungstoleranzen sind zu berücksichtigen.

### Beispiel für 630 × 0,10 mm

Kupferquerschnitt: $A_{\mathrm{Cu}}\approx4{,}95\,\mathrm{mm^2}$.

| $I_{\mathrm{RMS}}$ [A] | $J$ [A/mm²] | Bewertung |
|---:|---:|---|
| 20 | 4,0 | geeignet bei Luftkühlung |
| 30 | 6,1 | geeignet bei Luftkühlung |
| 35 | 7,1 | geeignet bei Luftkühlung |
| 40 | 8,1 | nur nach thermischem Nachweis |

### Vorgehensweise bei der Leiterauswahl

1. Erforderlichen RMS- und Spitzenstrom bestimmen.
2. Ziel-Stromdichte entsprechend der Kühlung festlegen.
3. Benötigten Kupferquerschnitt berechnen.
4. Geeignete Litze auswählen.
5. Mechanische Wickelbarkeit und Wickelraum prüfen.
6. AC- und DC-Verluste verifizieren.

### Hinweis

Die angegebenen Stromdichten sind Orientierungswerte. Die endgültige Dimensionierung muss anhand der thermischen Randbedingungen, der zulässigen Temperaturklasse der Isolation und der tatsächlichen AC-Verluste erfolgen.

---

## B1.5 Praxisleitfaden zur Leiterauswahl

### Ziel

Dieses Kapitel fasst die vorhergehenden Abschnitte zusammen und beschreibt eine systematische Vorgehensweise zur Auswahl geeigneter Leiter für Leistungsdrosseln und PFC-Anwendungen.

### Vorgehensweise

1. Elektrische Anforderungen festlegen: RMS-Strom, Spitzenstrom und Frequenz.
2. Erforderlichen Kupferquerschnitt aus der zulässigen Stromdichte bestimmen.
3. Einzeldrahtdurchmesser anhand der Skin-Tiefe auswählen.
4. Geeignete HF-Litze anhand des Kupferquerschnitts auswählen.
5. Mechanischen Außendurchmesser mit dem verfügbaren Wickelraum vergleichen.
6. DC- und AC-Verluste berechnen.
7. Temperaturanstieg simulieren oder messen.
8. Konstruktion iterativ optimieren.

### Vergleich gebräuchlicher HF-Litzen

| Litze | $A_{\mathrm{Cu}}$ [mm²] | typ. Außendurchmesser | empfohlener Dauerstrom* | Eignung bei 70 kHz | Bemerkung |
|---|---:|---:|---:|---|---|
| 160 × 0,10 | 1,26 | ≈ 2,0 mm | 5…10 A | sehr gut | kleine Drosseln |
| 210 × 0,10 | 1,65 | ≈ 2,3 mm | 7…13 A | sehr gut | kompakt |
| 400 × 0,10 | 3,14 | ≈ 3,2 mm | 12…22 A | sehr gut | mittlere Leistung |
| 630 × 0,10 | 4,95 | ≈ 4,0 mm | 20…35 A | sehr gut | 20-kW-PFC |
| 680 × 0,10 | 5,34 | ≈ 4,2 mm | 22…38 A | sehr gut | größerer Reservebereich |
| 800 × 0,10 | 6,28 | ≈ 4,6 mm | 25…45 A | sehr gut | hohe Strombelastung |

*Richtwerte bei forcierter Luftkühlung. Die tatsächliche Strombelastbarkeit hängt von Temperatur, Wicklungsaufbau und Kühlung ab.

### Entscheidungsmatrix

| Anforderung | Empfehlung | Begründung | Hinweis |
|---|---|---|---|
| maximaler Wirkungsgrad | kleiner Einzeldraht | geringe AC-Verluste | größerer Fertigungsaufwand |
| wenig Wickelraum | größere Stromdichte | kompakter Aufbau | Thermik prüfen |
| geringe Kosten | Massivdraht | einfache Fertigung | bei hoher Frequenz ungünstig |
| 70…100 kHz | 0,07…0,10 mm Einzeldrähte | passend zur Skin-Tiefe | Standard für HF-Litzen |

### Beispiel: 20-kW-PFC-Drossel

Für PFC-Drosseln im Bereich 20 kW und einer Schaltfrequenz von 70 kHz haben sich Litzen mit 630 × 0,10 mm oder 680 × 0,10 mm als geeigneter Kompromiss zwischen Kupferverlusten, Wickelbarkeit und Wickelraum erwiesen. Die endgültige Auswahl erfolgt anhand der thermischen Berechnung sowie der mechanischen Randbedingungen.

### Zusammenfassung

- Der Einzeldrahtdurchmesser wird primär durch die Schaltfrequenz bestimmt.
- Der Gesamtquerschnitt richtet sich nach RMS-Strom und zulässiger Stromdichte.
- Mechanische Randbedingungen sind ebenso wichtig wie elektrische Kriterien.
- Die endgültige Auslegung muss durch Verlust- und Temperaturberechnungen abgesichert werden.

---

# Band C – Magnetische Auslegung


---

## C1 Geometrische Kenngrößen

### Ziel

Dieses Kapitel definiert die geometrischen Kenngrößen magnetischer Kerne. Sie bilden die Grundlage für die Berechnung von Induktivität, Flussdichte, Reluktanz und Kernverlusten.

### Wichtige Geometrieparameter

| Symbol | Einheit | Bezeichnung | Bedeutung |
|---|---|---|---|
| $D_a$ | mm | Außendurchmesser | äußerer Kerndurchmesser |
| $D_i$ | mm | Innendurchmesser | Öffnung des Ringkerns |
| $h$ | mm | Kernhöhe | axiale Bauhöhe |
| $A_e$ | mm² | effektiver Querschnitt | wirksamer magnetischer Querschnitt |
| $l_e$ | mm | magnetische Weglänge | mittlere Flussweglänge |
| $V_e$ | mm³ | effektives Volumen | $A_e\cdot l_e$ |
| $A_L$ | nH/Wdg² | AL-Wert | Induktivität pro Windungsquadrat |

### Grundformeln

```math
D_m=\frac{D_a+D_i}{2}
```

```math
l_e\approx\pi D_m
```

```math
A_e=\frac{V_e}{l_e}
```

```math
V_e=A_e l_e
```

```math
L=A_LN^2
```

### Ringkerngeometrie

Bei Ringkernen verläuft der magnetische Fluss nahezu vollständig im Kernmaterial. Die mittlere magnetische Weglänge wird über den mittleren Durchmesser angenähert. Für präzise Berechnungen sind die effektiven Werte $A_e$, $l_e$ und $V_e$ aus dem Herstellerdatenblatt zu verwenden.

### Beispiel

| Parameter | Wert |
|---|---:|
| $D_a$ | 63,09 mm |
| $D_i$ | 31,70 mm |
| $h$ | 25,91 mm |
| $A_e$ | 360 mm² |
| $l_e$ | 144 mm |
| $V_e$ | 51 840 mm³ |

### Hinweise

- Für gestapelte Ringkerne addieren sich $A_e$ und $V_e$ annähernd proportional zur Anzahl der Kerne.
- Die magnetische Weglänge bleibt bei identischen gestapelten Ringkernen nahezu unverändert.
- Für Berechnungen sollten stets die effektiven Daten des Herstellers verwendet werden.

---

## C2 Der magnetische Kreis

### Ziel

Dieses Kapitel beschreibt den magnetischen Kreis als Analogon zum elektrischen Stromkreis. Die Zusammenhänge bilden die Grundlage für die Berechnung von Induktivität, Flussdichte und magnetischer Sättigung.

### Analogie zwischen elektrischem und magnetischem Kreis

| Elektrischer Kreis | Magnetischer Kreis | Einheit |
|---|---|---|
| Spannung $U$ | magnetomotorische Kraft $\Theta=N I$ | A |
| Strom $I$ | magnetischer Fluss $\Phi$ | Wb |
| Widerstand $R$ | Reluktanz $\mathcal{R}$ | A/Wb |
| Leitwert $G$ | Permeanz $P$ | Wb/A |

### Magnetomotorische Kraft

```math
\Theta=N\cdot I
```

### Reluktanz

```math
\mathcal{R}=\frac{l}{\mu_0\mu_rA}
```

Eine hohe Permeabilität oder ein großer Querschnitt verringern die Reluktanz. Eine größere magnetische Weglänge erhöht sie.

### Magnetischer Fluss

```math
\Phi=\frac{\Theta}{\mathcal{R}}
```

```math
B=\frac{\Phi}{A}
```

### Hopkinsonsches Gesetz

```math
\Theta=\Phi\cdot\mathcal{R}
```

### Luftspalt

Ein Luftspalt erhöht die Gesamtreluktanz deutlich, da Luft lediglich eine relative Permeabilität von ungefähr 1 besitzt. Dadurch sinkt die Induktivität. Gleichzeitig steigen Energiespeicherfähigkeit und Sättigungsreserve.

### Energiespeicherung

```math
W=\frac{1}{2}LI^2
```

Bei Speicherdrosseln wird der überwiegende Teil dieser Energie im Luftspalt gespeichert.

### Praktische Hinweise

- Pulverkerne besitzen einen verteilten Luftspalt und damit eine gleichmäßig verteilte Reluktanz.
- Ferritkerne benötigen häufig einen definierten mechanischen Luftspalt.
- Die magnetische Weglänge sollte grundsätzlich aus den Herstellerdaten übernommen werden.
- Bei hohen Flussdichten ist die Nichtlinearität der Permeabilität zu berücksichtigen.

---

## C3 Induktivitätsberechnung

### Ziel

Dieses Kapitel beschreibt die Berechnung der Induktivität magnetischer Bauteile auf Basis von Geometrie, Permeabilität und AL-Wert. Zusätzlich werden lineare und nichtlineare Induktivitäten erläutert.

### Grundgleichungen

```math
L=\frac{N^2}{\mathcal{R}}
```

```math
\mathcal{R}=\frac{l}{\mu_0\mu_rA_e}
```

```math
L=A_LN^2
```

```math
N=\sqrt{\frac{L}{A_L}}
```

### Herleitung

Aus dem magnetischen Fluss

```math
\Phi=\frac{\Theta}{\mathcal{R}}
```

und der Flussverkettung

```math
\Psi=N\Phi
```

folgt mit $L=\Psi/I$ und $\Theta=NI$ unmittelbar:

```math
L=\frac{N^2}{\mathcal{R}}
```

### Einflussgrößen

| Größe | Einfluss auf $L$ | Bemerkung |
|---|---|---|
| Windungszahl $N$ | quadratisch steigend | Verdopplung ergibt vierfache Induktivität |
| Permeabilität $\mu_r$ | linear steigend | nimmt bei Sättigung ab |
| Querschnitt $A_e$ | linear steigend | größerer Kern |
| Weglänge $l_e$ | linear fallend | längerer Flusspfad |
| Luftspalt | stark fallend | erhöht Energiespeicherung |

### Differential- und Sekanteninduktivität

Die Sekanteninduktivität beschreibt das Verhältnis $\Psi/I$ am Arbeitspunkt. Die Differentialinduktivität beschreibt die lokale Steigung $d\Psi/dI$ und ist insbesondere für Kleinsignalmodelle und Regelung relevant.

### Nichtlineare Kerne

Bei High-Flux-, Kool-Mu- und anderen Pulvermaterialien sinkt die effektive Permeabilität mit zunehmender Vormagnetisierung. Dadurch nimmt die Induktivität stromabhängig ab. Für Simulationen werden häufig B(H)-Kennlinien oder $\mu_r(B)$-Modelle verwendet.

### Beispiel

| Parameter | Beispiel |
|---|---:|
| $A_L$ | 95 nH/Wdg² |
| Windungszahl $N$ | 48 |
| $L=A_LN^2$ | ≈ 219 µH |

Der tatsächliche AL-Wert ist dem Datenblatt zu entnehmen.

### Auslegungshinweise

- Für lineare Ferritkerne kann häufig mit konstantem AL-Wert gerechnet werden.
- Bei Pulvermaterialien sollte die stromabhängige Induktivität berücksichtigt werden.
- Herstellerkennlinien sind einer vereinfachten Näherung vorzuziehen.
- Die Berechnung ist anschließend durch Messung oder FEM/PLECS zu verifizieren.

---

## C4 Flussdichte und magnetische Sättigung

### Ziel

Dieses Kapitel beschreibt die Berechnung der magnetischen Flussdichte sowie das Sättigungsverhalten magnetischer Werkstoffe. Die Flussdichte ist eine der wichtigsten Kenngrößen bei der Auslegung von Leistungsdrosseln und Transformatoren.

### Grundgrößen

| Symbol | Einheit | Bezeichnung | Beschreibung |
|---|---|---|---|
| $B$ | T | magnetische Flussdichte | magnetischer Fluss bezogen auf den Querschnitt |
| $\Phi$ | Wb | magnetischer Fluss | gesamter magnetischer Fluss |
| $H$ | A/m | magnetische Feldstärke | Erregung des Magnetfeldes |
| $B_{\mathrm{sat}}$ | T | Sättigungsflussdichte | maximal nutzbare Flussdichte |

### Grundgleichungen

```math
B=\frac{\Phi}{A_e}
```

```math
H=\frac{NI}{l_e}
```

```math
B=\mu_0\mu_rH
```

```math
\Phi=BA_e
```

### Magnetische Sättigung

Mit zunehmender Feldstärke nimmt die magnetische Flussdichte zunächst nahezu linear zu. Oberhalb eines materialspezifischen Bereichs sinkt die effektive Permeabilität, sodass weitere Stromerhöhungen nur noch geringe Änderungen der Flussdichte bewirken. Dieser Bereich wird als magnetische Sättigung bezeichnet.

### Typische Sättigungsflussdichten

| Material | $B_{\mathrm{sat}}$ typisch | Bemerkung |
|---|---:|---|
| MnZn-Ferrit | 0,30…0,50 T | geringe Kernverluste |
| High Flux | 1,5…1,8 T | hohe Energiespeicherung |
| Kool Mu | 0,8…1,0 T | Pulverkern mit verteiltem Luftspalt |
| MPP | 0,7…0,8 T | sehr geringe Verluste |
| Sendust | ≈ 1,0 T | gute DC-Vormagnetisierung |

### Auslegungsempfehlungen

- Ausreichenden Abstand zur Sättigungsflussdichte vorsehen.
- Temperaturabhängigkeit der Materialkennwerte berücksichtigen.
- Stromspitzen und Toleranzen in die Auslegung einbeziehen.
- Nichtlineare B(H)-Kennlinien für Simulationen verwenden.

### Beispiel

Für eine PFC-Drossel mit bekanntem Kernquerschnitt $A_e$ und magnetischer Weglänge $l_e$ kann die maximale Flussdichte aus Strom und Windungszahl berechnet werden. Der berechnete Wert ist anschließend mit der zulässigen Sättigungsflussdichte des gewählten Kernmaterials zu vergleichen.

### Zusammenfassung

- Die Flussdichte bestimmt die magnetische Ausnutzung des Kerns.
- Die Sättigungsflussdichte begrenzt den maximal zulässigen Strom.
- Pulverkerne besitzen aufgrund ihres verteilten Luftspalts ein günstiges Verhalten bei DC-Vormagnetisierung.
- Für präzise Auslegungen sind Herstellerkennlinien maßgebend.

---

## C5 Magnetische Kernmaterialien

### Ziel

Dieses Kapitel gibt einen Überblick über die wichtigsten magnetischen Kernmaterialien für Leistungsinduktivitäten und Transformatoren. Es vergleicht deren Eigenschaften, typische Einsatzgebiete sowie Vor- und Nachteile.

### Wichtige Materialkennwerte

| Kenngröße | Symbol | Einheit | Bedeutung |
|---|---|---|---|
| relative Permeabilität | $\mu_r$ | – | Maß für die magnetische Leitfähigkeit |
| Sättigungsflussdichte | $B_{\mathrm{sat}}$ | T | maximal nutzbare Flussdichte |
| Kernverlustdichte | $P_v$ | kW/m³ | Verlustleistung im Kern |
| Curie-Temperatur | $T_c$ | °C | Grenze der ferromagnetischen Eigenschaften |

### Vergleich wichtiger Kernmaterialien

| Material | $\mu_r$ typisch | $B_{\mathrm{sat}}$ | Verluste | DC-Vormagnetisierung | Typische Anwendung |
|---|---:|---:|---|---|---|
| MnZn-Ferrit | 1500…10000 | 0,30…0,50 T | sehr gering | gering | HF-Transformatoren |
| High Flux | 14…160 | 1,5…1,8 T | gering bis mittel | sehr gut | PFC- und Speicherdrosseln |
| Kool Mu | 14…125 | 0,8…1,0 T | gering | gut | EMV- und Speicherdrosseln |
| MPP | 14…550 | 0,7…0,8 T | sehr gering | gut | Präzisionsinduktivitäten |
| Sendust | 26…125 | ≈ 1,0 T | gering | gut | Leistungsdrosseln |
| nanokristallin | > 10000 | 1,2…1,3 T | sehr gering | mittel | Filter und Stromwandler |

### Auswahlkriterien

- erforderliche Induktivität und Strombelastbarkeit
- Schaltfrequenz und zulässige Kernverluste
- zulässige Flussdichte mit Sicherheitsreserve
- Temperaturbereich und Kühlung
- Kosten und Verfügbarkeit

### Werkstoffhinweise

**Ferrit:** Sehr geringe Kernverluste bei hohen Frequenzen, jedoch geringe Sättigungsflussdichte.

**High Flux:** Hohe Sättigungsflussdichte und gute DC-Vormagnetisierung, daher häufig für PFC-Drosseln geeignet.

**Kool Mu:** Verteilter Luftspalt und ausgewogene Kombination aus Verlusten und Energiespeicherung.

**MPP:** Sehr geringe Verluste und hohe Stabilität, jedoch vergleichsweise teuer.

**Nanokristallin:** Sehr hohe Permeabilität und geringe Verluste, besonders für Filter geeignet.

### Hinweise für die Auslegung

- Materialkennwerte ändern sich mit Temperatur und Flussdichte.
- Für präzise Berechnungen sind Herstellerdatenblätter und B(H)-Kennlinien maßgebend.
- Nichtlineare Materialmodelle verbessern die Genauigkeit von Simulationen.

### Zusammenfassung

Die Wahl des Kernmaterials beeinflusst Induktivität, Verluste, Baugröße und Wirkungsgrad maßgeblich. Die optimale Auswahl erfolgt anhand der elektrischen, thermischen und wirtschaftlichen Randbedingungen.

---

## C6 Kernverluste

### Ziel

Dieses Kapitel beschreibt die Entstehung von Kernverlusten, deren Berechnung sowie die wichtigsten Verlustmodelle für magnetische Werkstoffe. Die Kernverluste stellen insbesondere bei hohen Schaltfrequenzen einen wesentlichen Anteil der Gesamtverluste dar.

### Bestandteile der Kernverluste

| Verlustart | Ursache | Einflussgrößen |
|---|---|---|
| Hystereseverluste | Ummagnetisierung des Werkstoffs | Frequenz, Flussdichte, Material |
| Wirbelstromverluste | induzierte Ströme im Kern | Frequenz², Flussdichte, Material |
| Zusatzverluste | lokale Materialeffekte | Werkstoff, Temperatur |

### Klassische Steinmetz-Gleichung

```math
P_v=a\cdot B^b\cdot f^c
```

mit:

- $P_v$ = spezifische Kernverlustdichte
- $B$ = Spitzen- oder Spitz-Spitz-Flussdichte gemäß Herstellerdefinition
- $f$ = Ummagnetisierungsfrequenz
- $a$, $b$, $c$ = materialabhängige Steinmetz-Koeffizienten

### Erweiterte Verlustmodelle

| Modell | Anwendung |
|---|---|
| klassische Steinmetz-Gleichung | sinusförmige Anregung |
| Modified Steinmetz Equation (MSE) | PWM-Anregung |
| Improved Generalized Steinmetz Equation (iGSE) | beliebige periodische Flussverläufe |
| Herstellerkennlinien | höchste Genauigkeit |

### Einflussgrößen

- Schaltfrequenz
- Flussdichte
- Temperatur
- Material
- DC-Vormagnetisierung
- Wellenform der Flussdichte

### Vorgehensweise bei der Verlustberechnung

1. Material auswählen.
2. B(H)-Kennlinie bzw. Steinmetz-Koeffizienten beschaffen.
3. Flussdichteverlauf bestimmen.
4. Geeignetes Verlustmodell auswählen.
5. Kernverluste berechnen.
6. Temperaturanstieg verifizieren.

### Modified Steinmetz Equation (MSE)

Für PWM- und andere nichtsinusförmige Flussverläufe wird häufig die Modified Steinmetz Equation verwendet:

```math
P_v=\frac{1}{T}\int_0^T k_i\left|\frac{dB}{dt}\right|^\alpha(\Delta B)^{\beta-\alpha}\,dt
```

mit:

- $T=1/f$ = Periodendauer
- $dB/dt$ = zeitliche Änderung der Flussdichte
- $\Delta B$ = Spitzen-Spitzen-Flussdichte
- $\alpha$, $\beta$ = Steinmetz-Exponenten
- $k_i$ = materialabhängiger Koeffizient

### Improved Generalized Steinmetz Equation (iGSE)

Für beliebige periodische Flussdichteverläufe wird häufig die iGSE eingesetzt:

```math
P_v=\frac{1}{T}\int_0^T k_i\left|\frac{dB}{dt}\right|^\alpha(\Delta B)^{\beta-\alpha}\,dt
```

Der iGSE-Koeffizient kann geschrieben werden als:

```math
k_i=\frac{k}{(2\pi)^{\alpha-1}\int_0^{2\pi}|\cos\theta|^\alpha 2^{\beta-\alpha}\,d\theta}
```

### Hinweise für PFC-Drosseln

Bei PFC-Drosseln überlagern sich die 50-Hz-Grundschwingung und der hochfrequente Stromrippel. Dadurch entsteht ein nichtsinusförmiger Flussdichteverlauf. Für eine realitätsnahe Verlustabschätzung sind iGSE oder Herstellerkennfelder den klassischen Steinmetz-Ansätzen vorzuziehen.

### Zusammenfassung

- Die Flussdichte hat einen besonders großen Einfluss auf die Kernverluste.
- Die klassische Steinmetz-Gleichung eignet sich als erste Abschätzung.
- Für PWM-Anwendungen liefern iGSE oder Herstellerkennfelder häufig bessere Ergebnisse.
- Temperatur und DC-Vormagnetisierung sollten berücksichtigt werden.

---

## C7 Thermische Auslegung

### Ziel

Dieses Kapitel beschreibt die thermische Auslegung von Induktivitäten und Transformatoren. Ziel ist es, die entstehende Verlustleistung sicher an die Umgebung abzuführen und die maximal zulässige Bauteiltemperatur einzuhalten.

### Verlustquellen

| Verlustart | Formel | Bemerkung |
|---|---|---|
| Kupferverluste | $P_{\mathrm{Cu}}=I_{\mathrm{RMS}}^2R_{\mathrm{DC}}$ | gegebenenfalls inklusive AC-Zuschlag |
| Kernverluste | $P_{\mathrm{Fe}}=P_vV_e$ | $P_v$ aus Steinmetz oder Herstellerdaten |
| Gesamtverluste | $P_{\mathrm{ges}}=P_{\mathrm{Cu}}+P_{\mathrm{Fe}}$ | Grundlage der thermischen Auslegung |

### Thermisches Ersatzschaltbild

```math
\Delta T=P_{\mathrm{ges}}R_{\mathrm{th}}
```

```math
T_{\mathrm{Bauteil}}=T_{\mathrm{Umgebung}}+\Delta T
```

### Thermischer Widerstand

Der thermische Widerstand $R_{\mathrm{th}}$ beschreibt den Temperaturanstieg pro Watt Verlustleistung.

| Kühlart | Typischer $R_{\mathrm{th}}$ | Bemerkung |
|---|---|---|
| natürliche Konvektion | hoch | einfacher Aufbau |
| erzwungene Luftkühlung | mittel | typisch für PFC-Drosseln |
| Flüssigkeitskühlung | gering | hohe Leistungsdichte |

### Einflussgrößen

- Verlustleistung
- Luftgeschwindigkeit
- Kernoberfläche
- Wicklungsaufbau
- Einbaulage
- Umgebungstemperatur

### Beispiel

Gegeben seien $P_{\mathrm{ges}}=18\,\mathrm{W}$, $R_{\mathrm{th}}=3\,\mathrm{K/W}$ und eine Umgebungstemperatur von $40\,^\circ\mathrm{C}$.

```math
\Delta T=18\cdot3=54\,\mathrm{K}
```

```math
T_{\mathrm{Bauteil}}=40\,^\circ\mathrm{C}+54\,\mathrm{K}=94\,^\circ\mathrm{C}
```

### Hinweise für PFC-Drosseln

Bei leistungsstarken PFC-Drosseln sind Kern- und Wicklungsverluste gemeinsam zu betrachten. Die Temperatur sollte an Kern und Wicklung getrennt bewertet werden. Erzwungene Luftkühlung reduziert den thermischen Widerstand deutlich und ermöglicht höhere Dauerleistungen.

### Zusammenfassung

- Die thermische Auslegung basiert auf der Summe aller Verluste.
- Der thermische Widerstand verbindet Verlustleistung und Temperaturanstieg.
- Temperaturmessungen und Simulationen dienen zur Verifikation.
- Ausreichende Sicherheitsreserven erhöhen die Langzeitzuverlässigkeit.

---

## C8 Vollständige Auslegungsbeispiele

### Ziel

Dieses Kapitel fasst den kompletten Auslegungsablauf einer Leistungsdrossel zusammen. Anhand praxisnaher Beispiele werden die in den Kapiteln C1 bis C7 beschriebenen Berechnungsschritte angewendet.

### Typischer Auslegungsablauf

1. Elektrische Anforderungen festlegen: Leistung, Strom und Frequenz.
2. Benötigte Induktivität bestimmen.
3. Geeignetes Kernmaterial auswählen.
4. Kerngeometrie festlegen.
5. Windungszahl berechnen.
6. Flussdichte und Sättigungsreserve prüfen.
7. Leiterquerschnitt und Wicklungsaufbau bestimmen.
8. Kupfer- und Kernverluste berechnen.
9. Thermische Auslegung durchführen.
10. Messung oder Simulation zur Verifikation durchführen.

### Beispiel 1 – PFC-Drossel

| Parameter | Beispielwert |
|---|---|
| Topologie | dreiphasige Interleaved-PFC |
| Leistung | 20 kW Dauer / 40 kW Spitze |
| Netzspannung | 3 × 400 V |
| Zwischenkreisspannung | 750 V |
| Schaltfrequenz | 70 kHz |
| Kernmaterial | High Flux (Beispiel) |
| Kühlung | erzwungene Luftkühlung |

### Berechnungsschritte

- Induktivität aus Stromrippel und Schaltfrequenz bestimmen.
- Windungszahl aus AL-Wert oder Reluktanz berechnen.
- Flussdichte und Sättigungsreserve überprüfen.
- Kupferverluste einschließlich DC- und AC-Anteilen berechnen.
- Kernverluste mit Steinmetz, MSE oder iGSE bestimmen.
- Gesamtverluste und Temperaturanstieg berechnen.
- Ergebnisse mit Simulation und Messung vergleichen.

### Checkliste

- [x] Elektrische Anforderungen erfüllt
- [x] Sättigung ausgeschlossen
- [x] Kernverluste zulässig
- [x] Wicklung ausreichend dimensioniert
- [x] Temperaturgrenzen eingehalten
- [x] Mechanischer Aufbau geeignet
- [x] Fertigung möglich
- [x] Sicherheitsreserven vorhanden

### Typische Fehler

- zu geringe Sättigungsreserve
- Unterschätzung der AC-Kupferverluste
- Verwendung einer ungeeigneten Steinmetz-Gleichung
- nicht berücksichtigte Temperaturabhängigkeit
- zu geringer Wickelraum oder unzureichende Kühlung

### Zusammenfassung

Die Auslegung magnetischer Bauteile erfordert die gleichzeitige Betrachtung elektrischer, magnetischer und thermischer Randbedingungen. Erst die Kombination aus Berechnung, Simulation und Messung führt zu einer zuverlässigen und wirtschaftlichen Konstruktion.
