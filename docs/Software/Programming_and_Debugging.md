# Programmierung und Debugging

## Wichtiger Boardhinweis

GPIO19 und GPIO20 werden über den FSUSB42 wahlweise mit dem USB-Port oder dem CAN-Transceiver verbunden. Sobald die Firmware `USB_SEL` auf HIGH setzt, wird der Datenpfad auf CAN geschaltet. Die native USB-Verbindung wird dabei physikalisch getrennt.

## Empfohlener Ablauf

1. Firmware über die USB-Buchse flashen.
2. Zusätzlich einen USB-UART-Adapter am UART-Stecker anschließen.
3. UART-Konsole mit 115200 Baud, 8N1 öffnen.
4. Nach Aktivierung von CAN ausschließlich diese UART-Konsole für Debugging verwenden.

## UART-Anschluss

| Board | USB-UART-Adapter |
|---|---|
| GPIO43 / ESP_TXD | RX |
| GPIO44 / ESP_RXD | TX |
| GND | GND |

Nur 3,3-V-Logikpegel verwenden. Die 3,3-V-Versorgungsleitung des Adapters ist normalerweise nicht erforderlich.

## Konsequenzen

- Ein verschwundener USB-COM-Port nach Aktivierung von CAN ist erwartetes Verhalten.
- Debug-Ausgaben über `Serial`/USB sind dann nicht mehr zuverlässig verfügbar.
- Das Projekt verwendet deshalb `HardwareSerial(1)` auf GPIO43/GPIO44.
