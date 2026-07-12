# Änderungsanforderungen V9.1

## Umgesetzt

- Farben der Plus-/Minus- und Preset-Tasten zentral auf Mockup-Blau festgelegt.
- STOP-Taste zentral auf Rot und START-Taste auf Grün festgelegt.
- LVGL-Theme-Stile werden bei Buttons vollständig entfernt, damit keine Standardfarbe die definierte Palette überschreibt.
- Kommunikationsfehler-Punkt: grau ohne Fehler, rot bei lokalem RX-Timeout oder Statusbit 4.
- Preset-Tasten weiter nach links und oben verschoben und geometrisch zentriert.
- START-/STOP-Symbol und Beschriftung geometrisch zentriert.
- CAN-Monitor auf 20 getrennte, fest positionierte Zeilen umgestellt.
- Debug-Log auf 20 getrennte, fest positionierte Zeilen umgestellt; kein Zeilenumbruch und kein gegenseitiges Überschreiben.
- Verbindungswiederherstellung: Nach dem ersten gültigen Statusframe nach einem Timeout wird sofort ein Heartbeat-/Steuertelegramm gesendet. Das Kommunikationsstatusbit wird mit jedem neuen Frame neu ausgewertet und kann automatisch erlöschen.
- RS485-3,3-V-Hardwarekorrektur dokumentiert und Foto aufgenommen.
- Hinweis auf UART-Programmierung/Debugging bei aktivem CAN aufgenommen.
- Datenblattverzeichnis und Quellenliste ergänzt.
