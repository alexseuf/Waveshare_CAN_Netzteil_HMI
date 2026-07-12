# Bekannte Board-Besonderheiten und Korrekturen

## RS485-Transceiver U7 – unzulässiger 5-V-Ausgangspegel

Der SP3485EN wird im Originalschaltplan mit 5 V versorgt. Der Ausgang RO ist ohne Pegelwandler direkt mit dem 3,3-V-GPIO des ESP32-S3 verbunden. Bei 9600 Baud wurden Abstürze beobachtet.

### Bewährte Korrektur

1. Pin 8 (VCC) von U7 anheben.
2. Pin 8 mit Fädeldraht an 3,3 V anschließen.
3. Das zu U7 gehörende Beinchen von C63 anheben und mit dem neuen 3,3-V-Knoten verbinden.
4. Danach VCC und RO auf ungefähr 3,3 V kontrollieren.

Foto: `images/RS485_3V3_Fix.jpg`.

## USB/CAN-Multiplexer U13

- FSUSB42, OE dauerhaft LOW/aktiv.
- SEL LOW verbindet den gemeinsamen D+/D−-Port mit HSD1 (USB).
- SEL HIGH verbindet ihn mit HSD2 (CAN).
- Auf diesem Board gilt daher: `USB_SEL = HIGH` für CAN.

## Programmieren und Debuggen

GPIO19/20 werden über U13 zwischen USB und CAN umgeschaltet. Bei aktiviertem CAN wird die native USB-Datenverbindung getrennt. Dauerhafte Debug-Ausgaben müssen deshalb über den UART-Stecker erfolgen:

- GPIO43 / ESP_TXD -> RX des 3,3-V-USB-UART-Adapters
- GPIO44 / ESP_RXD -> TX des Adapters
- GND -> GND
- 115200 Baud, 8N1

Das Flashen kann vor der Umschaltung über USB erfolgen; für die Laufzeitdiagnose ist der UART-Port erforderlich.
