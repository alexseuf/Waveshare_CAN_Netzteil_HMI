# Waveshare CAN-Netzteil-HMI V9.3

PlatformIO-Projekt für das Waveshare ESP32-S3-Touch-LCD-4.3.

## Kernfunktionen
- RGB-LCD und GT911-Touch
- CAN 250 kbit/s über USB/CAN-Multiplexer (`USB_SEL = HIGH`)
- Sollwerttelegramm `0x1806E5F4` im 1-s-Zyklus
- Status-/Istwerttelegramm `0x18FF50E5`
- START/STOP, Presets, CC/CV, Energiezähler
- CAN-Monitor, Debug-Log und Einstellungen

## Toolchain
Die Plattform ist reproduzierbar auf PioArduino `55.03.39` festgeschrieben. Details: `docs/Toolchain.md`.

## Debug-UART
GPIO43 (TX), GPIO44 (RX), 115200 Baud, 3,3-V-Pegel.

## Dokumentation
Schaltplan, CAN-Protokoll, UI-Richtlinien und Änderungsanforderungen liegen im Ordner `docs`.

## Kritische Hardwarehinweise

- Bei aktiviertem CAN wird die native USB-Datenverbindung über den FSUSB42 getrennt. Zum Debuggen ist der UART-Stecker auf GPIO43/GPIO44 zu verwenden.
- U7 (SP3485EN) ist auf dem Originalboard mit 5 V versorgt und kann einen zu hohen RO-Pegel zum ESP32 liefern. Der bestätigte 3,3-V-Umbau ist in `docs/Board_Bugs_and_Fixes.md` beschrieben.


## Navigation V9.3

`HAUPT | SCOPE | LOG | INFO | ⚙`

Der CAN-Logger befindet sich unter INFO. Die Scope-Seite ist ein erster Live-Trend-Entwurf.
