#include "board_io.h"
#include "app_config.h"
#include <Arduino.h>
#include <Wire.h>
#include "debug_log.h"

namespace {
constexpr uint8_t CH422G_REG_MODE   = 0x24;
constexpr uint8_t CH422G_REG_OUTPUT = 0x38;
constexpr uint8_t EXIO_TP_RST       = 1U << 1; // EXIO1 CTP_RST
constexpr uint8_t EXIO_DISP         = 1U << 2; // EXIO2 DISP
constexpr uint8_t EXIO_LCD_RST      = 1U << 3; // EXIO3 LCD_RST
constexpr uint8_t EXIO_SD_CS        = 1U << 4; // EXIO4 SDCS
constexpr uint8_t EXIO_USB_SEL      = 1U << 5; // EXIO5 USB_SEL
uint8_t outputState = 0xFF;

bool writeByte(uint8_t address, uint8_t value) {
  Wire.beginTransmission(address);
  Wire.write(value);
  const uint8_t result = Wire.endTransmission(true);
  DebugLog::printf("[CH422G] write addr=0x%02X value=0x%02X result=%u\n", address, value, result);
  return result == 0;
}

bool applyOutput() {
  return writeByte(CH422G_REG_OUTPUT, outputState);
}
}

bool BoardIO::begin() {
  DebugLog::printf("[BOARD] I2C begin SDA=%d SCL=%d freq=%lu\n",
                AppConfig::PIN_I2C_SDA, AppConfig::PIN_I2C_SCL,
                static_cast<unsigned long>(AppConfig::I2C_FREQUENCY));
  Wire.begin(AppConfig::PIN_I2C_SDA, AppConfig::PIN_I2C_SCL, AppConfig::I2C_FREQUENCY);
  Wire.setTimeOut(AppConfig::I2C_TIMEOUT_MS);

  if (!writeByte(CH422G_REG_MODE, 0x01)) {
    DebugLog::println("[BOARD] ERROR: CH422G mode write failed");
    return false;
  }

  // FSUSB42: SEL=HIGH connects D+/D- to HSD2+/HSD2- (CAN).
  outputState = EXIO_TP_RST | EXIO_DISP | EXIO_LCD_RST | EXIO_SD_CS;
  if (AppConfig::USB_SEL_CAN_LEVEL_HIGH) outputState |= EXIO_USB_SEL;
  if (!applyOutput()) {
    DebugLog::println("[BOARD] ERROR: CH422G output write failed");
    return false;
  }

  pinMode(AppConfig::PIN_TOUCH_IRQ, INPUT_PULLUP);
  delay(20);
  return true;
}

void BoardIO::setTouchReset(bool high) {
  if (high) outputState |= EXIO_TP_RST;
  else outputState &= static_cast<uint8_t>(~EXIO_TP_RST);
  applyOutput();
  DebugLog::printf("[BOARD] CTP_RST=%s\n", high ? "HIGH" : "LOW");
}

void BoardIO::prepareTouchAddressPin(bool high) {
  pinMode(AppConfig::PIN_TOUCH_IRQ, OUTPUT);
  digitalWrite(AppConfig::PIN_TOUCH_IRQ, high ? HIGH : LOW);
  DebugLog::printf("[BOARD] GPIO4/INT address strap=%s\n", high ? "HIGH" : "LOW");
}

void BoardIO::releaseTouchInterruptPin() {
  pinMode(AppConfig::PIN_TOUCH_IRQ, INPUT_PULLUP);
  delay(5);
  DebugLog::printf("[BOARD] GPIO4/INT released, level=%d\n", digitalRead(AppConfig::PIN_TOUCH_IRQ));
}

void BoardIO::setBacklight(bool on) {
  if (on) outputState |= EXIO_DISP;
  else outputState &= static_cast<uint8_t>(~EXIO_DISP);
  applyOutput();
  DebugLog::printf("[BOARD] DISP=%s\n", on ? "ON" : "OFF");
}

void BoardIO::scanI2c() {
  DebugLog::println("[I2C] scan start");
  uint8_t found = 0;
  for (uint8_t address = 1; address < 127; ++address) {
    Wire.beginTransmission(address);
    const uint8_t result = Wire.endTransmission(true);
    if (result == 0) {
      DebugLog::printf("[I2C] device 0x%02X", address);
      if (address == 0x5D || address == 0x14) DebugLog::print(" GT911");
      if (address == 0x24 || address == 0x38) DebugLog::print(" CH422G");
      DebugLog::println();
      ++found;
    }
    delay(1);
  }
  DebugLog::printf("[I2C] scan end, found=%u\n", found);
}


bool BoardIO::selectCanMode() {
  if (AppConfig::USB_SEL_CAN_LEVEL_HIGH) outputState |= EXIO_USB_SEL;
  else outputState &= static_cast<uint8_t>(~EXIO_USB_SEL);
  const bool ok = applyOutput();
  DebugLog::printf("[MUX] CAN-Modus USB_SEL=%s EXIO=0x%02X %s\n",
                   AppConfig::USB_SEL_CAN_LEVEL_HIGH ? "HIGH" : "LOW", outputState, ok ? "OK" : "FEHLER");
  return ok;
}

bool BoardIO::selectUsbMode() {
  if (AppConfig::USB_SEL_CAN_LEVEL_HIGH) outputState &= static_cast<uint8_t>(~EXIO_USB_SEL);
  else outputState |= EXIO_USB_SEL;
  const bool ok = applyOutput();
  DebugLog::printf("[MUX] USB-Modus EXIO=0x%02X %s\n", outputState, ok ? "OK" : "FEHLER");
  return ok;
}

unsigned char BoardIO::exioState() { return outputState; }
