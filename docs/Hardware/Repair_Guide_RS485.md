# Reparatur- und Modifikationshinweise

## RS485-Umbau auf 3,3 V

Benötigt werden eine feine Lötspitze, Flussmittel, dünner isolierter Fädeldraht, Pinzette und ein Multimeter.

1. Versorgung trennen und Kondensatoren entladen lassen.
2. U7 Pin 8 anheben, ohne das Gehäuse oder Nachbarpins zu beschädigen.
3. C63 auf der bisherigen 5-V-Seite anheben.
4. U7 Pin 8 und den angehobenen Anschluss von C63 gemeinsam an einen geprüften 3,3-V-Punkt führen.
5. Widerstand gegen GND kontrollieren und anschließend unter Spannung 3,3 V messen.
6. RO-Pegel bei 9600 Baud prüfen.

Referenzfoto: `images/RS485_3V3_Fix.jpg`.
