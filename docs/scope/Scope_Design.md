# Scope-/Trend-Funktion V9.4.2

Die Scope-Seite zeichnet die CAN-Istwerte von Spannung, Strom und Leistung auf und stellt sie als Live-Trend dar.

## Datenspeicherung

- Abtastung im 1-s-Raster
- Ringpuffer mit 1800 gemeinsamen Datensätzen
- Aufzeichnungsdauer: maximal 30 Minuten
- Jeder Datensatz enthält Zeitstempel, Spannung, Strom und Leistung
- Nach 1800 Datensätzen wird der jeweils älteste Eintrag überschrieben
- Kommunikationsunterbrechungen werden als ungültige Punkte und damit als Lücken dargestellt

## Zeitdarstellung

Das Diagramm besitzt 180 feste Darstellungspositionen. Die gespeicherten Messwerte werden anhand ihres tatsächlichen Alters auf diese Positionen abgebildet. Der neueste Wert liegt bei 0 s. Deshalb erscheint ein gleich langes Ereignis im 10-Minuten-Fenster doppelt so breit wie im 20-Minuten-Fenster. Wenn für ein gewähltes Fenster noch nicht genügend historische Daten vorhanden sind, bleibt der ältere linke Bereich leer.

Der einstellbare Zeitbereich reicht von 5 s bis 30 min.

## Bedienung

- Minus/Plus verändern das Zeitfenster schrittweise.
- Horizontales Wischen mit einem Finger auf der X-Achse verändert das Zeitfenster kontinuierlich.
- Wischen nach rechts vergrößert das Zeitfenster; Wischen nach links verkleinert es.
- Vertikales Wischen in der grünen Y-Achsenspalte skaliert Spannung U.
- Vertikales Wischen in der blauen Y-Achsenspalte skaliert Strom I.
- Vertikales Wischen in der orangefarbenen Y-Achsenspalte skaliert Leistung P.
- Nach oben wischen verkleinert den dargestellten Wertebereich und zoomt hinein; nach unten wischen vergrößert den Wertebereich und zoomt heraus.
- STANDARD setzt Zeitfenster und Y-Skalierungen auf 10 min, 60 V, 20 A und 1000 W zurück.
- PAUSE/FORTSETZEN hält die Aufzeichnung an bzw. setzt sie fort.
- LÖSCHEN leert den Ringpuffer.

## Anzeige

- Grün: Spannung U
- Blau: Strom I
- Orange: Leistung P
- Die drei Messgrößen werden auf eine gemeinsame interne Diagrammskala von 0 bis 1000 normiert.
- Die farbigen Achsenwerte zeigen die aktuellen absoluten Skalen.
- Die Legende ist innerhalb der Plotfläche verschiebbar und kann nicht in die Achsenbeschriftungen gezogen werden.

## Geplante Erweiterungen

- Pan im pausierten Zustand
- Messcursor mit Zeitpunkt und Absolutwerten
- Umschaltbare Kanäle
- Speicherung längerer Aufzeichnungen auf SD-Karte
