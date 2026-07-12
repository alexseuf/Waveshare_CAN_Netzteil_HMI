# CAN-Protokoll – bestätigte Erkenntnisse

## Bus

- CAN 2.0B Extended Frames
- Baudrate: 250 kbit/s

## BMS/Controller → Netzteil

- ID: `0x1806E5F4`
- Zyklus: 1000 ms
- Funktion: Sollwerte und Heartbeat
- Byte 1–2: Sollspannung, Big Endian, 0,1 V/Bit
- Byte 3–4: Sollstrom, Big Endian, 0,1 A/Bit
- Byte 5: 0 = Start, 1 = Stop/Batterieschutz

Beispiel 53,3 V / 10,0 A / Start:

`02 15 00 64 00 00 00 00`

Wenn das Netzteil ca. 5–6 s kein gültiges Telegramm empfängt, setzt es Statusbit 4 und stoppt.

## Netzteil → Broadcast

- ID: `0x18FF50E5`
- Zyklus: ungefähr 1000 ms
- Byte 1–2: Ausgangsspannung, Big Endian, 0,1 V/Bit
- Byte 3–4: Ausgangsstrom, Big Endian, 0,1 A/Bit
- Byte 5: Status

Statusbits:

- Bit 0: Hardwarefehler
- Bit 1: Übertemperatur
- Bit 2: Netzspannungsfehler
- Bit 3: Batterie fehlt oder ist verpolt
- Bit 4: Kommunikations-Timeout

Bestätigte Beispiele:

- `0x08`: Batterie fehlt/verpolt
- `0x18`: Batterie fehlt/verpolt und Kommunikations-Timeout
