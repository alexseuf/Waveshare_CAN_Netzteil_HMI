# Changelog

## 9.1.0

- Buttonfarben durch Entfernen aller LVGL-Theme-Stile deterministisch gemacht.
- Plus/Minus und Presets blau, START grün, STOP rot.
- Preset-Position korrigiert.
- Kommunikationsfehler erholt sich nach wiederhergestellter CAN-Verbindung; sofortiger Heartbeat bei Reconnect.
- CAN- und Debug-Log auf getrennte Zeilenobjekte umgestellt, dadurch keine Überlappungen.
- RS485-3,3-V-Fix, UART-Debughinweis und Datenblattquellen dokumentiert.

## V9.0.0
- PioArduino auf 55.03.39 festgeschrieben.
- UI in `ui_core.cpp`, `ui_main.cpp`, `ui_can.cpp`, `ui_log.cpp` und `ui_settings.cpp` modularisiert.
- Farben an das freigegebene Mockup angepasst.
- Plus/Minus-Tasten größer und neu positioniert.
- START/STOP vollständig zentriert.
- Preset-Tasten zentriert; Hinweistext gekürzt.
- CC/CV zeigt nur noch den aktiven Zustand.
- Energiefeld vergrößert.
- Istwerte und Leistung werden bei Kommunikationsausfall als `-` angezeigt.
- CAN-Monitor auf bis zu 16 Zeilen erweitert.
- Debug-Log auf bis zu 18 Einträge erweitert und Überlappung behoben.
- Dokumentation vollständig aktualisiert.

## 9.2.0

- RGB565-Datenpinreihenfolge gemäß Farbtest korrigiert.
- Preset- und START/STOP-Ausrichtung überarbeitet.
- Robuste CAN-Recovery inklusive TWAI Bus-Off-Recovery.
- Neue prozentuale CC/CV-Erkennung mit Hysterese.
- SCOPE/Trend-Entwurf ergänzt.
- CAN-Logger unter INFO verschoben.
- Zeitstempel HH:MM:SS und kollisionsfreie Log-Zeilen.
- Hardwaredokumentation und Datenblätter erweitert.

## 9.3.0
- Spannungssollbereich auf 51,2 bis 58,4 V begrenzt.
- Sollwertfelder neu ausgerichtet; Spannungsfeld verbreitert.
- START/STOP-Symbole vergrößert, Abstand erhöht und komplette Tastenfläche direkt auf PRESSED ausgewertet.
- Scope maximiert, untere Statuszeile entfernt, Achsenbeschriftungen und verschiebbare Legende ergänzt.
- Zeitbasisbedienung neben der Anzeige sowie Standardansicht-Taste ergänzt.
- Zwei-Finger-Zoom: jeweilige Achse über Achsenbereich, alle Achsen im Plotbereich.
