# Projektprotokoll

## 2026-07-11

- RGB-Display und LVGL erfolgreich in Betrieb genommen.
- GT911 funktioniert mit eigenem Treiber ohne I²C-Fehler.
- CAN-Baudrate mit Oszilloskop und Sniffer als 250 kbit/s bestätigt.
- USB/CAN-Multiplexer analysiert: CAN benötigt `USB_SEL = HIGH`.
- CAN-Senden und -Empfangen erfolgreich.
- Status `0x08` ohne angeschlossene Batterie bestätigt.
- Nach Ausfall des 1-s-Heartbeat wird nach ca. 6 s Status `0x18` gesendet.
- V6 korrigiert die fehlenden Zahlen auf Display und Preset-Tasten.

## 2026-07-11 – V7

- Hauptseite an neues Mockup angepasst.
- Große blaue Plus-/Minus-Tasten und große START-/STOP-Tasten umgesetzt.
- Umlaute durch eingebettete LVGL-Schriften korrigiert.
- CC/CV-Anzeige ergänzt; Modus ist eine Ableitung aus Soll-/Istwerten.
- CAN-Monitor vergrößert und bedienbar gemacht.

## V8
- Hauptseite bleibt frei von Log-Einträgen.
- Debug-Log erhält eine eigene LOG-Seite.
- Energiezähler wird aus Uout × Iout integriert und nach 5 min Unterbrechung gelöscht.
- Navigation: HAUPT | CAN | LOG | INFO | Zahnrad.

## V9 – UI-Konsolidierung und reproduzierbare Toolchain
- Toolchain auf PioArduino 55.03.39 festgeschrieben.
- UI modularisiert.
- Mockup-Farben und Bedienelementausrichtung korrigiert.
- CAN-/Debug-Log-Flächen besser genutzt.

## V9.1

- Ursache der falschen Buttonfarben eingegrenzt: LVGL-Theme-/Standardstyles konnten lokale Farbvorgaben überlagern. Buttons entfernen nun sämtliche Theme-Stile und erhalten die vollständige Palette explizit für Default- und Pressed-State.
- CAN-Reconnect verbessert: sofortiger Heartbeat nach erstem wieder empfangenen Statusframe.
- CAN- und Debug-Zeilen werden als einzelne Labels mit fester Höhe dargestellt.
- RS485-Hardwarefehler dokumentiert: SP3485EN-VCC/C63 von 5 V auf 3,3 V umbauen.
- UART als zwingender Debugpfad bei aktivem CAN dokumentiert.

## V9.2

- Zweistufiger Farbtest bestätigt: Phase B besitzt die korrekte Rot-/Blau-Datenleitungzuordnung.
- RGB-Pinreihenfolge im Displaytreiber entsprechend korrigiert.
- CAN-Link-Recovery um Bus-Off-Recovery und drei gültige Statusframes zur Rückkehr nach ONLINE erweitert.
- CC/CV-Erkennung auf prozentuale Soll-/Ist-Abweichungen mit Hysterese umgestellt.
- Scope-/Trend-Entwurf und CAN-Logger als INFO-Unterseite ergänzt.
- Log-Zeitstempel auf HH:MM:SS umgestellt.
