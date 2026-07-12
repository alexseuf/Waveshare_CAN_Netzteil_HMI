# Bedienoberfläche V9.4.2

## Hauptseite

- Sollspannung: 51,2 bis 58,4 V, Schrittweite 0,1 V.
- Sollstrom: 0,0 bis 20,0 A, Schrittweite 0,1 A.
- Große blaue Plus-/Minus-Tasten; Gedrückthalten wiederholt die Änderung.
- START mit Play-Symbol und STOP mit Stop-Symbol.
- Presets: kurz tippen lädt, langes Drücken speichert.
- Statuspunkte: grau/inaktiv und grün/aktiv.
- Fehlerpunkte: grau/kein Fehler und rot/Fehler aktiv.
- Bei fehlender Kommunikation werden Istwerte und Leistung als Strich dargestellt.

## CC/CV-Anzeige

Das CAN-Protokoll liefert kein eigenes CC/CV-Bit. Die Anzeige wird aus den relativen Abweichungen zwischen Soll- und Istwerten abgeleitet. Diejenige Regelgröße mit der kleineren prozentualen Abweichung bestimmt den angezeigten Modus. Eine Hysterese von 0,25 Prozentpunkten verhindert schnelles Umschalten. Ohne bestätigten Ladebetrieb wird AUS angezeigt.

Die Anzeige ist damit eine abgeleitete Betriebszustandsanzeige und kein direkt vom Ladegerät übermitteltes Statusbit.

## Energie

Die Energie wird aus Ausgangsspannung × Ausgangsstrom über die Zeit integriert. Nach fünf Minuten ohne bestätigten Ladebetrieb wird der Zähler automatisch gelöscht. Über EINSTELLUNGEN kann er auch manuell zurückgesetzt werden.

## Scope

- 1800 Datensätze bei 1-s-Abtastung ergeben maximal 30 Minuten Aufzeichnung.
- Spannung, Strom und Leistung werden mit echter Zeitposition dargestellt.
- Die Achsenskalierung erfolgt mit einem Finger direkt auf der jeweiligen Achse.
- Die Taste STANDARD stellt die Ausgangsskalierung wieder her.
- Die Legende zeigt kurze farbige Linien für U, I und P.

## Schrift und Umlaute

Eigene LVGL-Schriften unterstützen insbesondere ä, ö, ü, Ä, Ö, Ü, ß, ° und ±.

## CAN-Logger und Debug-Log

- CAN-Logger unter INFO mit 18 sichtbaren Frames.
- Debug-Log auf eigener LOG-Seite mit 18 sichtbaren Einträgen.
- Pause/Fortsetzen und Löschen.
- Zeitstempel im Format HH:MM:SS.
- Getrennte RX-, TX- und Fehlerzähler im CAN-Logger.
