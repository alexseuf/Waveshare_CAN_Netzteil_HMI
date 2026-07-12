# Änderungsumfang V9.2 gegenüber V9.1

- RGB-Datenleitungen gemäß erfolgreicher Phase B des Farbtests korrigiert.
- Plus/Minus- und Preset-Schaltflächen erscheinen dadurch blau, STOP und Fehlerpunkte rot.
- Preset-Schaltflächen über Flex-Layout mathematisch horizontal und vertikal zentriert.
- START/STOP-Symbol und Text als gemeinsame, zentrierte Gruppe angeordnet.
- CAN-Recovery um Bus-Off-Erkennung, TWAI-Recovery und Link-Zustände OFFLINE/RECOVERING/ONLINE/BUS-OFF erweitert.
- Kommunikationsfehler wird nach drei gültigen Statusframes ohne Statusbit 4 automatisch gelöscht.
- CC/CV-Erkennung vergleicht die prozentuale Abweichung von Strom und Spannung; 0,25 Prozentpunkte Hysterese verhindern Flattern.
- Zeitstempel in CAN-Logger und Debug-Log: HH:MM:SS.
- Debug-Log-Zeilenhöhe wird aus der tatsächlichen Font-Höhe berechnet.
- Neue SCOPE-Seite als funktionsfähiger Entwurf mit Ringpuffer, Spannung/Strom/Leistung, Pause, Löschen und vier Zeitfenstern.
- CAN-Logger aus der Hauptnavigation entfernt und als Unterseite unter INFO/DIAGNOSE eingebaut.
- Dokumentation und Datenblätter aktualisiert.
