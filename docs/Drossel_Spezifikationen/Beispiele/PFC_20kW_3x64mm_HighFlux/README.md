---
title: "Beispielprojekt PFC-Drossel 20 kW"
version: "3.0"
status: "Entwurfs- und Berechnungsstand"
---

# PFC-Drossel 20 kW

**3 × 64-mm-High-Flux-Ringkern · 26 Windungen · 680 × 0,10-mm-HF-Litze**

Dieses Beispiel wurde aus der Entwicklungsspezifikation für eine dreiphasige 20-kW-Interleaved-PFC-Drossel in eine modulare GitHub-Markdown-Struktur übertragen.

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

Weitere Kapitel der Word-Spezifikation werden schrittweise ergänzt.

## Hinweis zum Entwicklungsstand

Die dokumentierten Berechnungswerte sind Entwurfswerte. Insbesondere die stromabhängige Induktivität, die Einheitensetzung der Steinmetzparameter, die HF-Wicklungsverluste und das thermische Verhalten sind vor einer Bauteilfreigabe durch Messung beziehungsweise projektspezifische Simulation zu verifizieren.
