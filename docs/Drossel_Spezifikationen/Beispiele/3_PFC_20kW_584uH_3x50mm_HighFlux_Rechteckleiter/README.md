---
title: "PFC-Drossel 584 µH – 3 × 50,8 mm High Flux"
version: "Revision 1.1"
status: "Entwurfs- und Berechnungsstand"
---

# PFC-Drossel 584 µH

**3 × Magnetics 0058717A2 · High Flux 26 µ · 80 Windungen · Rechteckleiter 0,72 × 4,00 mm**

## Lesen

- [Gesamtausgabe](Gesamtausgabe.md)  
  **Zuletzt aktualisiert:** 03.08.2026

## Projektübersicht

| Parameter | Wert |
|---|---:|
| Zielinduktivität | ca. 584 µH |
| Kern | 3 × Magnetics 0058717A2 |
| Kernabmessung | 50,80 × 31,75 × 13,46 mm je Kern |
| Material | High Flux, µ = 26 |
| Windungszahl | 80 |
| Leiter | Rechteckkupfer 0,72 × 4,00 mm |
| Kupferquerschnitt | 2,88 mm² |
| Ersatz-Runddurchmesser | 1,915 mm |
| Nennleistung | 20 kW dauerhaft |
| Spitzenleistung | 40 kW für 0,5 s |
| Netzspannung | 400 V Leiter-Leiter |
| Zwischenkreisspannung | 750 V |
| Schaltfrequenz | 70 kHz |

## Neu berechnete Abbildungen

1. [Mechanischer Aufbau](Bilder/abbildung_01_mechanischer_aufbau.svg)
2. [B(H)-Kennlinie](Bilder/abbildung_03_bh_kennlinie.svg)
3. [Differentielle und Sekanteninduktivität](Bilder/abbildung_04_induktivitaet.svg)
4. [Differentielle und Sekantenpermeabilität](Bilder/abbildung_05_permeabilitaet.svg)
5. [DC-Kupferverluste](Bilder/abbildung_06_kupferverluste.svg)
6. [Stromwelligkeit](Bilder/abbildung_07_stromwelligkeit.svg)
7. [Flussdichtehub](Bilder/abbildung_08_flussdichtehub.svg)
8. [Vorläufige Gesamtverluste](Bilder/abbildung_09_gesamtverluste.svg)
9. [Temperaturabschätzung](Bilder/abbildung_10_temperaturabschaetzung.svg)
10. [Einlagiger Rechteckleiter-Wicklungsaufbau](Bilder/abbildung_14_wicklungsaufbau_rechteckleiter_einlagig.svg)

Die magnetischen Kennlinien basieren derzeit auf einer glatten Sättigungsnäherung mit $L(0)=584\,\mu\mathrm H$, $\mu_{r0}=26$ und $B_{sat}=1{,}65\,\mathrm T$. Widerstands-, Gesamtverlust- und Temperaturdiagramme sind als vorläufige Auslegungswerte gekennzeichnet und müssen durch Wickelmuster, Herstellerdaten und Messungen validiert werden.

## Kapitel

1. [Zweck und Geltungsbereich](01_Zweck_und_Geltungsbereich.md)
2. [Systemanforderungen](02_Systemanforderungen.md)
3. [Magnetischer Aufbau](03_Magnetischer_Aufbau.md)
4. [Wicklungsaufbau](04_Wicklungsaufbau.md)
5. [Mechanischer Aufbau](05_Mechanischer_Aufbau.md)
6. [Berechnungsgrundlagen](06_Berechnungsgrundlagen.md)
7. [Magnetische Kennlinien](07_Magnetische_Kennlinien.md)
8. [Stromwelligkeit und Flussdichtehub](08_Stromwelligkeit_und_Flussdichtehub.md)
9. [Elektrische Wicklungsverluste](09_Elektrische_Wicklungsverluste.md)
10. [Kernverluste](10_Kernverluste.md)
11. [Gesamtverluste](11_Gesamtverluste.md)
12. [Thermik](12_Thermik.md)
13. [Fertigung](13_Fertigung.md)
14. [Prüfung](14_Pruefung.md)
15. [PLECS-/MATLAB-Parametersatz](15_PLECS_MATLAB.md)
16. [Bewertung](16_Bewertung.md)
17. [Quellen](17_Quellen.md)
