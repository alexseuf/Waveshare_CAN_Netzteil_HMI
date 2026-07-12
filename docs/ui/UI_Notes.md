# Bedienoberfläche V7

## Hauptseite

- Sollspannung: 0,0 bis 58,4 V, Schrittweite 0,1 V.
- Sollstrom: 0,0 bis 20,0 A, Schrittweite 0,1 A.
- Große blaue Plus-/Minus-Tasten. Gedrückthalten wiederholt die Änderung.
- START mit Play-Symbol, STOP mit Stop-Symbol.
- Presets: kurz tippen lädt, langes Drücken speichert.
- Statuspunkte: grau/inaktiv, grün/aktiv.
- Fehlerpunkte: grau/kein Fehler, rot/Fehler aktiv.

## CC/CV-Anzeige

Das CAN-Protokoll liefert keinen eigenen CC/CV-Modus. Die Anzeige wird deshalb aus Soll- und Istwerten abgeleitet:

- **CC**: Iststrom mindestens 95 % des Sollstroms und Ausgangsspannung mehr als 0,2 V unter Sollspannung.
- **CV**: Ausgangsspannung höchstens 0,15 V unter Sollspannung.
- **--**: Ausgang aus, Anlauf oder Zustand nicht eindeutig.

Die Anzeige ist damit eine plausible Betriebszustandsanzeige, jedoch kein direkt vom Ladegerät übermitteltes Statusbit.

## Schrift und Umlaute

V7 enthält eigene LVGL-Schriften mit deutschen Zeichen. Unterstützt werden insbesondere ä, ö, ü, Ä, Ö, Ü, ß, ° und ±.

## CAN-Monitor

- Größere Schrift und Nutzung der gesamten Inhaltsfläche.
- Neun gut lesbare Frames mit Zeit, Richtung, ID und Datenbytes.
- Pause/Fortsetzen und Löschen des lokalen Logs.
- Separate RX-/TX-/Fehlerzähler.
