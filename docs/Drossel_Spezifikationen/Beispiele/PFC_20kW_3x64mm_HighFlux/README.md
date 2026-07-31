---
title: "Beispielprojekt PFC-Drossel 20 kW"
version: "3.0"
status: "Entwurfs- und Berechnungsstand"
---

# PFC-Drossel 20 kW

**3 × 64-mm-High-Flux-Ringkern · 26 Windungen · 680 × 0,10-mm-HF-Litze**

Dieses Beispiel wurde aus der Entwicklungsspezifikation für eine dreiphasige 20-kW-Interleaved-PFC-Drossel in eine modulare GitHub-Markdown-Struktur übertragen.

## Lesen

- [Gesamtausgabe](Gesamtausgabe.md)  
  **Zuletzt aktualisiert:** 31.07.2026, 08:31 Uhr CEST

## Projektübersicht

| Parameter | Wert |
|---|---:|
| Dokumentversion | 3.0 |
| Status | Entwurfs- und Berechnungsstand |
| Nennbetrieb | 20 kW, 3 × 400 V AC, 750 V DC |
| Kurzzeitüberlast | 40 kW für 0,5 s |
| Schaltfrequenz | 70 kHz |
| Kernaufbau | 3 gestapelte High-Flux-Ringkerne |
| Wicklung | 26 Windungen, 680 × 0,10 mm HF-Litze |

## Kapitel

1. [Zweck und Geltungsbereich](01_Zweck_und_Geltungsbereich.md)
2. [Systemanforderungen](02_Systemanforderungen.md)
3. [Magnetischer Aufbau](03_Magnetischer_Aufbau.md)
4. [Wicklungsaufbau](04_Wicklungsaufbau.md)
5. [Mechanischer Aufbau und Befestigung](05_Mechanischer_Aufbau.md)
6. [Berechnungsgrundlagen](06_Berechnungsgrundlagen.md)
7. [Magnetische Kennlinien](07_Magnetische_Kennlinien.md)
8. [Stromwelligkeit und Flussdichtehub](08_Stromwelligkeit_und_Flussdichtehub.md)
9. [Elektrische Wicklungsverluste](09_Elektrische_Wicklungsverluste.md)
10. [Kernverluste](10_Kernverluste.md)
11. [Gesamtverluste](11_Gesamtverluste.md)
12. [Thermische Anforderungen](12_Thermische_Anforderungen.md)
13. [Fertigungsanforderungen](13_Fertigungsanforderungen.md)
14. [Prüf- und Abnahmekriterien](14_Pruef_und_Abnahmekriterien.md)
15. [PLECS-/MATLAB-Parametersatz](15_PLECS_MATLAB_Parametersatz.md)
16. [Bewertung und offene Verifikation](16_Bewertung_und_offene_Verifikation.md)
17. [Quellenbasis](17_Quellenbasis.md)

## Hinweis zum Entwicklungsstand

Die dokumentierten Berechnungswerte sind Entwurfswerte. Insbesondere die stromabhängige Induktivität, die Einheitensetzung der Steinmetzparameter, die HF-Wicklungsverluste und das thermische Verhalten sind vor einer Bauteilfreigabe durch Messung beziehungsweise projektspezifische Simulation zu verifizieren.