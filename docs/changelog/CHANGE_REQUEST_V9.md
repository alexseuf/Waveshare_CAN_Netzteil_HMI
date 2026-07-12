# Änderungsanforderungen V9

Status: umgesetzt in V9.0.0.

- Farben der Plus/Minus-, Preset-, START- und STOP-Tasten an das freigegebene Mockup angeglichen.
- Plus/Minus-Tasten größer und nach links/oben korrigiert.
- START/STOP-Inhalt per Flex-Layout vollständig zentriert; Symbole über LVGL-Symbolfont.
- Preset-Hinweis: `Kurz: Laden` / `Lang: Speichern`.
- Preset-Tasten horizontal und vertikal zentriert.
- CC/CV-Feld zeigt nur den aktuellen Zustand AUS, CC oder CV.
- Energiefeld vergrößert.
- Bei Kommunikationsausfall zeigen Istspannung, Iststrom und Leistung `-`.
- Fehlerpunkte schalten von Grau auf Rot.
- CAN-Monitor zeigt bis zu 16 Zeilen.
- Debug-Log zeigt bis zu 18 Einträge mit fester Zeilenhöhe ohne Überlappung.
- Debug-Log bleibt ausschließlich auf der LOG-Seite.
- Zahnrad bleibt ganz rechts und besitzt eine große Touchfläche.
- Toolchain auf PioArduino 55.03.39 festgeschrieben.
- UI-Code in mehrere Dateien modularisiert.
