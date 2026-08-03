# Beispiele für Drossel-Spezifikationen

Dieser Ordner enthält vollständig ausgearbeitete Beispielprojekte auf Basis realer Entwicklungsspezifikationen. Jedes Beispiel besitzt eine Projektübersicht, Einzelkapitel, einen Bilderordner und eine eigene Gesamtausgabe.

## Beispielprojekte

### 1_ PFC-Drossel 20 kW – 3 × 64-mm-High-Flux-Ringkern – R_DC,25 = 27,71 mΩ

**Kernaufbau:** 3 gestapelte High-Flux-Ringkerne  
**Wicklung:** 26 Windungen, einlagig, 680 × 0,10 mm HF-Litze  
**Wicklungswiderstand:** 27,71 mΩ bei 25 °C; 38,06 mΩ bei 120 °C  
**Betrieb:** 20 kW Dauerleistung, 40 kW für 0,5 s

- [Projektübersicht](PFC_20kW_3x64mm_HighFlux/README.md)
- [Gesamtausgabe](PFC_20kW_3x64mm_HighFlux/Gesamtausgabe.md)
- [Kapitel](PFC_20kW_3x64mm_HighFlux/README.md#kapitel)
- [Bilder](PFC_20kW_3x64mm_HighFlux/Bilder)

### 2_ PFC-Drossel 525 µH – 3 × Magnetics C058110A2 – R_DC,25 = 23,20 mΩ

**Kernaufbau:** 3 × C058110A2, High Flux 60 µ  
**Wicklung:** 48 Windungen, zweilagig 28 + 20, 630 × 0,10 mm HF-Litze  
**Wicklungswiderstand:** 23,20 mΩ bei 25 °C; 31,86 mΩ bei 120 °C  
**Betrieb:** 20 kW Dauerleistung, 40 kW für 0,5 s

- [Projektübersicht](2_PFC_20kW_525uH_3x56mm_HighFlux_23mOhm/README.md)
- [Gesamtausgabe](2_PFC_20kW_525uH_3x56mm_HighFlux_23mOhm/Gesamtausgabe.md)
- [Kapitel](2_PFC_20kW_525uH_3x56mm_HighFlux_23mOhm/README.md#kapitel)
- [Bilder](2_PFC_20kW_525uH_3x56mm_HighFlux_23mOhm/Bilder)

### 3_ PFC-Drossel 584 µH – 3 × Magnetics 0058717A2 – Rechteckleiter

**Kernaufbau:** 3 × 0058717A2, High Flux 26 µ  
**Wicklung:** 80 Windungen, einlagiger Rechteckleiter 0,72 × 4,00 mm  
**Zielinduktivität:** 584 µH  
**Betrieb:** 20 kW Dauerleistung, 40 kW für 0,5 s  
**Abbildungen:** mechanischer Aufbau, Wicklungsaufbau, B(H), L(I), µ(I), Stromwelligkeit, Flussdichtehub, Verluste und Temperaturabschätzung

- [Projektübersicht](3_PFC_20kW_584uH_3x50mm_HighFlux_Rechteckleiter/README.md)
- [Gesamtausgabe](3_PFC_20kW_584uH_3x50mm_HighFlux_Rechteckleiter/Gesamtausgabe.md)
- [Kapitel](3_PFC_20kW_584uH_3x50mm_HighFlux_Rechteckleiter/README.md#kapitel)
- [Bilder](3_PFC_20kW_584uH_3x50mm_HighFlux_Rechteckleiter/Bilder)

## Verzeichnisstruktur

```text
Beispiele/
├── README.md
├── PFC_20kW_3x64mm_HighFlux/
├── 2_PFC_20kW_525uH_3x56mm_HighFlux_23mOhm/
└── 3_PFC_20kW_584uH_3x50mm_HighFlux_Rechteckleiter/
    ├── README.md
    ├── Gesamtausgabe.md
    ├── 01_...md bis 17_...md
    └── Bilder/
```

## Zweck

Die Beispiele zeigen, wie die modulare Vorlage unter `../Vorlage/` für konkrete Drosselprojekte ausgefüllt wird. Sie dienen als Referenz für Struktur, Tabellen, Formeln, Berechnungsannahmen, Prüfmerkmale und offene Verifikationspunkte.
