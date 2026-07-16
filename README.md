<div align="center">

# Waveshare CAN-Netzteil-HMI

### Touch-HMI für ein CAN-gesteuertes Ladegerät auf Basis des Waveshare ESP32-S3-LCD-4.3

**Aktueller Teststand:** `V9.5.3-test1`  
**Stabile Version auf `main`:** `V9.4.2`

</div>

---

## Projektübersicht

Dieses PlatformIO-Projekt macht das Waveshare ESP32-S3-Touch-LCD-4.3 zu einer eigenständigen Bedien- und Diagnoseeinheit für ein CAN-gesteuertes Netzteil beziehungsweise Ladegerät.

Das Gerät übernimmt:

- Anzeige von Spannung, Strom, Leistung, Energie und Betriebszustand
- Vorgabe von Sollspannung und Sollstrom
- START/STOP-Steuerung
- Überwachung der CAN-Kommunikation
- grafische Aufzeichnung der Messwerte
- Fehler- und Debug-Anzeige
- ab V9.5 zusätzlich WiFi, NTP, MQTT, Home Assistant und lokalen Webzugriff

> Die Ladegeräte- und CAN-Funktion bleibt auch bei Ausfall von WLAN, MQTT, NTP oder Webserver vollständig lauffähig.

---

## Bedienoberfläche

<table>
<tr>
<td width="50%" valign="top">

### Hauptansicht

<img src="docs/images/hmi_main_screen_v9_4_2.jpg" alt="Hauptansicht des HMI" width="100%">

Die Hauptansicht zeigt die wichtigsten Ist- und Sollwerte. Spannung und Strom können direkt eingestellt werden. START, STOP, Presets, CC/CV-Zustand und Energiezähler sind ohne Untermenü erreichbar.

</td>
<td width="50%" valign="top">

### Scope und Trendanzeige

<img src="docs/images/hmi_scope_screen_v9_4_2.jpg" alt="Scope-Ansicht des HMI" width="100%">

Die Scope-Seite zeichnet Spannung, Strom und Leistung als zeitlichen Verlauf auf. Die X- und Y-Achsen lassen sich direkt mit einem Finger skalieren. Eine verschiebbare Legende ordnet die Messkurven eindeutig zu.

</td>
</tr>
</table>

---

## Einstellungsmenü V9.5

<img src="docs/images/v9_5_settings_mockup.svg" alt="Mockup des neuen V9.5 Einstellungsmenüs" width="100%">

| Bereich | Funktion | Stand |
|---|---|---|
| **Debug Settings** | Log-Level, Diagnoseanzeigen, FPS, RAM, PSRAM und CPU-Last | umgesetzt |
| **WiFi** | Netzwerksuche, Zugangsdaten, Status, Signalstärke, IP, NTP und AP-Fallback | Testversion umgesetzt |
| **MQTT / Home Assistant** | Broker, Discovery, Availability/LWT und optionale Fernsteuerung | folgt in Stufe 3 |

### WiFi-/NTP-Stufe 2

Die Testversion `9.5.3-test1` enthält:

- asynchrone WLAN-Netzwerksuche
- Auswahl des WLANs per Touch
- Passwortfeld mit Bildschirmtastatur
- Speicherung der Zugangsdaten in NVS
- automatische Verbindung und Wiederverbindung
- Anzeige von SSID, RSSI, Signalqualität, IP, Gateway und MAC-Adresse
- Access-Point-Fallback `Charger-HMI-Setup`
- NTP-Synchronisation mit deutscher Sommer-/Winterzeit
- keine blockierende Abhängigkeit der CAN- oder Ladefunktion vom Netzwerk

---

## Hardware

<table>
<tr>
<td width="50%" valign="top">

### Waveshare ESP32-S3-LCD-4.3

<img src="docs/images/waveshare_esp32_s3_lcd_4_3_board.jpg" alt="Waveshare ESP32-S3-LCD-4.3" width="100%">

Das Board kombiniert ESP32-S3, 800×480-RGB-Display, Touchcontroller und PSRAM. Der CAN-Betrieb erfolgt über den auf dem Board vorhandenen USB/CAN-Multiplexer.

</td>
<td width="50%" valign="top">

### CAN-Netzteil

<img src="docs/images/charger_top_fan.jpg" alt="CAN-Netzteil mit Lüfter" width="100%">

Das HMI sendet die Sollwerte zyklisch über CAN und verarbeitet die Status- und Istwerttelegramme des Netzteils. Kommunikationsausfälle werden erkannt und sicher behandelt.

</td>
</tr>
</table>

---

## CAN-Kommunikation

| Funktion | CAN-ID | Verhalten |
|---|---:|---|
| Sollwert- und Steuertelegramm | `0x1806E5F4` | wird im 1-s-Zyklus gesendet |
| Status- und Istwerttelegramm | `0x18FF50E5` | liefert Spannung, Strom und Status |
| Bitrate | `250 kbit/s` | Extended CAN |

Die Firmware startet immer im sicheren STOP-Zustand. Das zyklische Steuertelegramm wird trotzdem als Heartbeat weitergesendet.

---

## Navigation

```text
HAUPT | SCOPE | LOG | INFO | ⚙
```

- **HAUPT:** Sollwerte, Istwerte und START/STOP
- **SCOPE:** zeitlicher Verlauf von Spannung, Strom und Leistung
- **LOG:** Debug-Ausgaben
- **INFO:** Systemstatus und CAN-Monitor
- **⚙:** Debug-, WiFi- und später MQTT-Einstellungen

---

## Entwicklungszweige

```text
main                     stabile V9.4.2
feature/v9.5             allgemeine V9.5-Entwicklung
test/v9.5.2-ui-fixes     aktueller Teststand V9.5.3-test1 mit WiFi/NTP
```

### Dokumentation

- [Entwicklungsplan V9.5](docs/V9.5_PLAN.md)
- [Schritt 1: Einstellungsmenü](docs/V9.5_STEP1.md)
- [Stufe 2: WiFi und NTP](docs/V9.5.3_TEST1_WIFI.md)
- [UI-Spezifikation](docs/V9.5_UI_SPEC.md)
- [Changelog](CHANGELOG.md)

---

## Toolchain

- PlatformIO
- Arduino Framework
- PioArduino `55.03.39`
- LVGL `9.5.0`
- ESP32-S3 mit 16 MB Flash und OPI-PSRAM

## Debug-Schnittstelle

```text
TX: GPIO43
RX: GPIO44
Baudrate: 115200
Pegel: 3,3 V
```

Bei aktiviertem CAN wird die native USB-Datenverbindung über den FSUSB42 getrennt. Für Laufzeitdiagnose und Bootmeldungen ist deshalb der separate Debug-UART vorgesehen.

## Kritischer Hardwarehinweis

U7 (`SP3485EN`) ist auf dem Originalboard mit 5 V versorgt und kann dadurch einen zu hohen RO-Ausgangspegel zum ESP32 liefern. Der bestätigte Umbau auf 3,3 V ist in [`docs/Board_Bugs_and_Fixes.md`](docs/Board_Bugs_and_Fixes.md) dokumentiert.
