# 11.1 Ausnutzung Kernmaterial

Die maximale magnetische Flussdichte im Kern setzt sich aus der Flussdichte der 50-Hz-Grundwelle und dem überlagerten hochfrequenten Flussdichtehub der PWM-Anregung zusammen.

## 11.1.1 Flussdichte der Grundwelle

Für die x-Achse wird der Effektivwert des Grundwellenstroms verwendet. Der zugehörige Stromscheitelwert lautet

$$
\hat I_1=\sqrt{2}\,I_{1,\mathrm{rms}}.
$$

Die Grundwellen-Flussdichte wird aus der in Kapitel 7 dokumentierten B(H)-Kennlinie bestimmt. Für die Darstellung wurden die dort angegebenen Stützstellen verwendet und linear interpoliert:

| Stromscheitelwert $\hat I_1$ | Grundwellen-Flussdichte $B_{\max,\mathrm{GW}}$ |
|---:|---:|
| 0,0 A | 0,000 T |
| 28,9 A | 0,663 T |
| 40,8 A | 0,849 T |
| 57,7 A | 1,038 T |
| 81,6 A | 1,230 T |
| 90,0 A | 1,281 T |

Der kleine Offset des empirischen B(H)-Fits bei $H\rightarrow0$ wurde für diese Ausnutzungsdarstellung nicht übernommen; physikalisch wird $B(0)=0$ gesetzt.

## 11.1.2 Überlagerung des 70-kHz-Flussdichtehubs

Aus Kapitel 8 ergibt sich für die betrachteten elektrischen Randbedingungen

$$
\Delta B_{pp,\max}=129{,}175\,\mathrm{mT}.
$$

Für die maximale positive Flussdichte ist die halbe Spitze-Spitze-Welligkeit zur Grundwellen-Flussdichte zu addieren:

$$
\Delta B_{pk,\max}
=
\frac{\Delta B_{pp,\max}}{2}
=
64{,}588\,\mathrm{mT}.
$$

Damit lautet die zweite Kennlinie

$$
B_{\max,\mathrm{gesamt}}(I_{1,\mathrm{rms}})
=
B_{\max,\mathrm{GW}}(I_{1,\mathrm{rms}})
+
64{,}588\,\mathrm{mT}.
$$

Der Abstand zwischen beiden Kennlinien ist daher über den gesamten dargestellten Strombereich konstant. Die rote Kennlinie ist gegenüber der blauen Kennlinie exakt um $64{,}588\,\mathrm{mT}$ parallel nach oben verschoben.

![Abbildung 13: Ausnutzung des Kernmaterials](Bilder/abbildung_13_ausnutzung_kernmaterial_bmax.svg)

*Abbildung 13: Maximale magnetische Flussdichte über dem Grundwellen-Effektivstrom. Blau: 50-Hz-Grundwelle aus der B(H)-Kennlinie. Rot: Grundwelle zuzüglich $\Delta B_{pp,\max}/2=64{,}588\,\mathrm{mT}$ für den 70-kHz-Anteil.*

## 11.1.3 Arbeitspunkte

| Betriebspunkt | $I_{1,\mathrm{rms}}$ | $\hat I_1$ | $B_{\max,\mathrm{GW}}$ | $B_{\max,\mathrm{gesamt}}$ | Ausnutzung bezogen auf $B_{sat}=1{,}78\,\mathrm{T}$ |
|---|---:|---:|---:|---:|---:|
| 20 kW Dauerbetrieb | 28,87 A | 40,83 A | 0,849 T | 0,914 T | 51,3 % |
| 40 kW Spitzenbetrieb | 57,74 A | 81,66 A | 1,230 T | 1,295 T | 72,7 % |

Die verbleibende rechnerische Reserve zur Modell-Sättigungsflussdichte beträgt damit:

- bei 20 kW etwa $1{,}78-0{,}914=0{,}866\,\mathrm{T}$,
- bei 40 kW etwa $1{,}78-1{,}295=0{,}485\,\mathrm{T}$.

## 11.1.4 Einordnung

Der konstante vertikale Abstand gilt für die hier verwendete Hüllkurve mit dem maximalen PWM-Flussdichtehub. Der tatsächliche 70-kHz-Flussdichtehub hängt über den Tastgrad vom Netzwinkel ab und liegt gemäß Kapitel 8 zwischen $31{,}193$ und $129{,}175\,\mathrm{mT}_{pp}$. Für eine konservative Bewertung der maximalen Flussdichte wird in diesem Kapitel der Maximalwert verwendet.

Am stromlosen Betriebspunkt ist die rote Kurve deshalb als theoretische Hüllkurve unter fortgesetzter PWM-Anregung zu verstehen. Wird die PWM im Leerlauf abgeschaltet, verschwindet auch der hochfrequente Flussdichteanteil.

Die B(H)-Kennlinie und damit die dargestellten Grundwellenwerte beruhen auf dem verwendeten empirischen Modell. Die endgültige Kernausnutzung ist mit Messungen oder dem realen PLECS-Flussdichteverlauf zu verifizieren.
