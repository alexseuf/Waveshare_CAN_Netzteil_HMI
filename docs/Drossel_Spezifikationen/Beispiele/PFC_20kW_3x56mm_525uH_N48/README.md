---
title: "PFC-Drossel 525 µH – 3 × 56 mm – 48 Windungen"
version: "Revision 4.3"
status: "Entwurfs- und Berechnungsstand"
---

# PFC-Drossel 525 µH

**3 × Magnetics 0058111A2 · High Flux 60 µ · 48 Windungen · 630 × 0,10-mm-HF-Litze**

Dieser Ordner enthält das Beispiel für die 20-kW-PFC-Drossel mit drei gestapelten Ringkernen der 56-mm-Klasse und einer Anfangsinduktivität von ungefähr 525 µH.

## Projektübersicht

| Parameter | Wert |
|---|---:|
| Nennleistung | 20 kW dauerhaft |
| Kurzzeitüberlast | 40 kW für 0,5 s |
| Netzspannung | 400 V Leiter-Leiter |
| Zwischenkreisspannung | 750 V |
| Schaltfrequenz | 70 kHz |
| Kernaufbau | 3 × Magnetics 0058111A2 |
| Kernmaterial | High Flux 60 µ |
| Außendurchmesser | 57,15 mm |
| Innendurchmesser | 35,56 mm |
| Höhe je Kern | 13,97 mm |
| Gesamthöhe | 41,91 mm |
| Effektiver Kernquerschnitt | 432 mm² |
| Effektive Weglänge | 143 mm |
| Effektives Kernvolumen | 61,78 cm³ |
| Windungszahl | 48 |
| Wicklungsaufbau | zweilagig, 28 + 20 Windungen |
| Litze | 630 × 0,10 mm |
| Anfangsinduktivität | ca. 525 µH |
| RDC bei 25 °C | 23,2 mΩ |
| RDC bei 120 °C | 31,9 mΩ |

## Geplante Kapitelstruktur

1. Zweck und Geltungsbereich
2. Systemanforderungen
3. Magnetischer Aufbau
4. Wicklungsaufbau
5. Mechanischer Aufbau
6. Berechnungsgrundlagen
7. Magnetische Kennlinien
8. Stromwelligkeit und Flussdichtehub
9. Elektrische Wicklungsverluste
10. Kernverluste
11. Gesamtverluste
12. Thermische Anforderungen
13. Fertigungsanforderungen
14. Prüf- und Abnahmekriterien
15. PLECS-/MATLAB-Parametersatz
16. Bewertung und offene Verifikation
17. Quellenbasis

## Berechnungsstand

Die Revision 4.3 verwendet für die Kernverlustberechnung die Katalogkonvention

$$
P_v[\mathrm{mW/cm^3}] = 246{,}54 \cdot B[\mathrm{T}]^{2{,}218} \cdot f[\mathrm{kHz}]^{1{,}311}.
$$

Der über eine Netzperiode gemittelte Kernverlust beträgt im zugrunde liegenden Dokument 4,0 W. Der maximale momentane Rechenwert beträgt 9,2 W.

## Hinweis

Die Werte stammen aus der Entwicklungsspezifikation Revision 4.3. Der reale Flussdichtehub, die AC-Wicklungsverluste und die thermische Anbindung sind vor einer Freigabe durch Simulation und Messung zu verifizieren.
