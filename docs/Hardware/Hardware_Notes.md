# Hardware-Notizen – Waveshare ESP32-S3-Touch-LCD-4.3

## USB/CAN-Multiplexer

Der ESP32-S3 teilt GPIO19 und GPIO20 zwischen USB und CAN über U13 (`FSUSB42UMX`).

- GPIO20: CAN TX
- GPIO19: CAN RX
- CH422G EXIO5 steuert `USB_SEL`
- `OE` des FSUSB42 liegt dauerhaft LOW, der Schalter ist somit aktiviert.
- `USB_SEL = LOW`: HSD1, USB-Buchse
- `USB_SEL = HIGH`: HSD2, CAN-Transceiver

Die für CAN bestätigte Einstellung ist daher **EXIO5 = HIGH**.

## Debug-Schnittstelle

Da USB nach dem Umschalten auf CAN nicht mehr für die Debugkonsole verfügbar ist:

- GPIO43 / ESP_TXD → RX des USB-UART-Adapters
- GPIO44 / ESP_RXD ← TX des USB-UART-Adapters
- GND ↔ GND
- 115200 Baud, 8N1, 3,3-V-Pegel

## CAN-Transceiver

- U12: TJA1051T/3
- 250 kbit/s bestätigt
- CANH/CANL am Steckverbinder J4
- Abschlusswiderstand über Jumper J7/R15 (120 Ohm)

## Touch

- Controller: GT911
- SDA: GPIO8
- SCL: GPIO9
- IRQ: GPIO4
- Reset: CH422G EXIO1
- Eigener GT911-Treiber wird verwendet.
