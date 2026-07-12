# UI-Design-Richtlinien

## Ziel
Industrielle, kontrastreiche Bedienoberfläche für 800 × 480 Pixel auf 96 × 54 mm aktiver Displayfläche. Bedienung muss mit dem Finger sicher möglich sein.

## Navigation
- Reihenfolge: **HAUPT | CAN | LOG | INFO | Zahnrad**.
- Zahnrad ganz rechts, mindestens 72 × 36 Pixel Touchfläche.
- Aktiver Reiter orange, inaktive Reiter dunkelgrau.
- Debug-Log erscheint ausschließlich auf der LOG-Seite.

## Farben
- Hintergrund: nahezu schwarz `#020304`.
- Panels: `#080A0D`, Rahmen `#4B5057`.
- Überschriften und Einstelltasten: Blau `#0877E5` / Dunkelblau `#064CA8`.
- Systemstatus: Grün `#20D52A`.
- Fehlerüberschrift: Blau; Fehlerpunkte inaktiv grau, aktiv rot `#E31918`.
- START: Grün, STOP: Rot, jeweils weiße Schrift.
- CC: Orange, CV: Grün, AUS: Weiß/Grau.

## Bedienelemente
- Plus/Minus mindestens 54 × 64 Pixel, blau mit weißer Schrift.
- START und STOP jeweils etwa 275 × 64 Pixel.
- Play- und Stop-Symbol getrennt vom Text mit LVGL-Symbolfont darstellen, damit keine leeren Kästchen entstehen.
- Kurzer Druck verändert Sollwerte um 0,1 V/A; Halten aktiviert Wiederholung.

## Werte
- Sollwerte mit einer Nachkommastelle.
- Istwerte mit zwei Nachkommastellen.
- Leistung mit einer Nachkommastelle.
- Einstellbereich in kleiner, heller Schrift unter dem Sollwert.
- Deutsche Umlaute müssen über die eingebetteten UTF-8-Fonts korrekt dargestellt werden.

## CC/CV und Energie
- Nur drei Betriebszustände: AUS, CC, CV.
- Energiezähler in kWh rechts im selben Panel.
- Energieintegration aus Istspannung × Iststrom.
- Automatisches Löschen, wenn der gemessene Ladevorgang länger als fünf Minuten unterbrochen ist.

## Status
- Systemstatuspunkte: grau oder grün.
- Fehlerpunkte: grau oder rot.
- Gleichmäßige vertikale Abstände innerhalb beider Gruppen.

## CAN-Monitor und Debug-Log
- Nutzen nahezu die komplette Inhaltsfläche.
- Mindestens 16-Pixel-Schrift.
- CAN RX grün, CAN TX cyan, Warnung orange, Fehler rot.
- Pause/Fortsetzen und Löschen vorhanden.
- LOG-Kategorien im Zahnradmenü einzeln ein- und ausschaltbar.


## V9.1: deterministische Button-Styles

Vor dem Setzen der Buttonfarben werden sämtliche Theme-Styles entfernt. Plus/Minus und Presets verwenden ausschließlich Mockup-Blau, START ausschließlich Grün und STOP ausschließlich Rot. Status- und Fehlerpunkte werden in jedem UI-Zyklus explizit aktualisiert.
