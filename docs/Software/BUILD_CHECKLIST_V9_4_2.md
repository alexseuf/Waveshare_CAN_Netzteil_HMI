# Build- und Abnahmecheckliste V9.4.2

## Build und Version

- [ ] PioArduino 55.03.39 wird verwendet.
- [ ] LVGL 9.5.0 wird verwendet.
- [ ] PlatformIO Clean und anschließend Build laufen fehlerfrei durch.
- [ ] INFO zeigt Firmware 9.4.2.
- [ ] UART-Startmeldung zeigt NETZTEIL-HMI v9.4.2.

## Hauptseite und Kommunikation

- [ ] Sollspannung ist auf 51,2 bis 58,4 V begrenzt.
- [ ] Sollstrom ist auf 0,0 bis 20,0 A begrenzt.
- [ ] START, STOP, Plus/Minus und Presets reagieren korrekt.
- [ ] Kommunikationsausfall zeigt ungültige Istwerte als Strich.
- [ ] CAN arbeitet mit 250 kbit/s und 1-s-Heartbeat.
- [ ] CAN-Recovery funktioniert nach Timeout und Bus-Off.
- [ ] CC/CV-Anzeige und Energiezähler reagieren plausibel.

## Scope

- [ ] Ringpuffer zeichnet einen Messpunkt pro Sekunde auf.
- [ ] 10- und 20-Minuten-Ansicht zeigen Ereignisse mit korrektem Breitenverhältnis.
- [ ] X-Achse lässt sich mit einem Finger horizontal skalieren.
- [ ] Spannung, Strom und Leistung lassen sich in ihren Y-Achsenspalten getrennt vertikal skalieren.
- [ ] Achsenwerte aktualisieren sich während der Bedienung.
- [ ] STANDARD setzt 10 min, 60 V, 20 A und 1000 W wieder her.
- [ ] Legende lässt sich verschieben und überdeckt keine Achsenbeschriftungen.
- [ ] PAUSE/FORTSETZEN und LÖSCHEN funktionieren.

## Log, Hardware und Dokumentation

- [ ] CAN-Logger und Debug-Log zeigen je 18 Zeilen ohne Überlappung.
- [ ] Zeitstempel werden als HH:MM:SS dargestellt.
- [ ] Debugging erfolgt über UART GPIO43/44 mit 115200 Baud und 3,3-V-Pegel.
- [ ] USB/CAN-Umschaltung mit USB_SEL = HIGH für CAN funktioniert.
- [ ] Hardwarehinweise, Schaltplan, CAN-Protokoll und Datenblätter sind im docs-Verzeichnis vorhanden.
