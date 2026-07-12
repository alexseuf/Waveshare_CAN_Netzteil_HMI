# CC/CV-Erkennung

Das CAN-Protokoll liefert kein eigenes CC/CV-Bit. V9.2 leitet den Zustand aus der relativen Soll-/Ist-Abweichung ab:

- Strom prozentual näher am Sollwert als Spannung: CC.
- Spannung prozentual näher am Sollwert als Strom: CV.
- Bei weniger als 0,25 Prozentpunkten Unterschied bleibt der vorherige Zustand erhalten.
- Bei STOP, Kommunikationsfehler, fehlender Ladebereitschaft oder sehr kleinem Iststrom: AUS.
