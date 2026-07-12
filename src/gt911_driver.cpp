#include "gt911_driver.h"
#include "app_config.h"
#include "board_io.h"
#include <Arduino.h>
#include <Wire.h>
#include <cstring>
#include "debug_log.h"

namespace {
constexpr uint16_t REG_COMMAND       = 0x8040;
constexpr uint16_t REG_CONFIG_START  = 0x8047;
constexpr uint16_t REG_X_OUTPUT_MAX  = 0x8048;
constexpr uint16_t REG_PRODUCT_ID    = 0x8140;
constexpr uint16_t REG_FIRMWARE      = 0x8144;
constexpr uint16_t REG_STATUS        = 0x814E;
constexpr uint16_t REG_FIRST_POINT   = 0x814F;
constexpr uint8_t MAX_POINTS         = 5;
constexpr uint8_t POINT_BYTES        = 8;

// GT911 address selection commonly used by Waveshare/Goodix:
// INT LOW during reset release -> 0x5D
// INT HIGH during reset release -> 0x14
bool strapHighForAddress(uint8_t address) {
  return address == 0x14;
}
}

bool GT911Driver::probe(uint8_t address) {
  Wire.beginTransmission(address);
  const uint8_t result = Wire.endTransmission(true);
  DebugLog::printf("[GT911] probe 0x%02X -> %u\n", address, result);
  return result == 0;
}

bool GT911Driver::selectAddressAndReset(uint8_t address) {
  DebugLog::printf("[GT911] reset/address sequence for 0x%02X\n", address);

  BoardIO::setTouchReset(false);
  BoardIO::prepareTouchAddressPin(strapHighForAddress(address));
  delay(20);

  BoardIO::setTouchReset(true);
  delay(10);

  // Goodix recommends holding INT strap briefly after reset rises.
  delay(50);
  BoardIO::releaseTouchInterruptPin();
  delay(100);

  return probe(address);
}

bool GT911Driver::readRegister(uint16_t reg, uint8_t *data, size_t length) {
  if (address_ == 0 || data == nullptr || length == 0) return false;

  size_t offset = 0;
  while (offset < length) {
    const size_t chunk = min(static_cast<size_t>(32), length - offset);
    const uint16_t currentReg = static_cast<uint16_t>(reg + offset);

    Wire.beginTransmission(address_);
    Wire.write(static_cast<uint8_t>(currentReg >> 8));
    Wire.write(static_cast<uint8_t>(currentReg & 0xFF));
    const uint8_t tx = Wire.endTransmission(false);
    if (tx != 0) {
      ++readErrors_;
      DebugLog::printf("[GT911][READ] address phase failed reg=0x%04X code=%u\n", currentReg, tx);
      return false;
    }

    const size_t got = Wire.requestFrom(static_cast<int>(address_), static_cast<int>(chunk), static_cast<int>(true));
    if (got != chunk) {
      ++readErrors_;
      DebugLog::printf("[GT911][READ] data failed reg=0x%04X expected=%u got=%u\n",
                    currentReg, static_cast<unsigned>(chunk), static_cast<unsigned>(got));
      while (Wire.available()) Wire.read();
      return false;
    }

    for (size_t i = 0; i < chunk; ++i) data[offset + i] = static_cast<uint8_t>(Wire.read());
    offset += chunk;
  }
  return true;
}

bool GT911Driver::writeRegister(uint16_t reg, const uint8_t *data, size_t length) {
  if (address_ == 0 || data == nullptr || length == 0) return false;

  size_t offset = 0;
  while (offset < length) {
    const size_t chunk = min(static_cast<size_t>(28), length - offset);
    const uint16_t currentReg = static_cast<uint16_t>(reg + offset);

    Wire.beginTransmission(address_);
    Wire.write(static_cast<uint8_t>(currentReg >> 8));
    Wire.write(static_cast<uint8_t>(currentReg & 0xFF));
    Wire.write(data + offset, chunk);
    const uint8_t result = Wire.endTransmission(true);
    if (result != 0) {
      ++writeErrors_;
      DebugLog::printf("[GT911][WRITE] failed reg=0x%04X code=%u\n", currentReg, result);
      return false;
    }
    offset += chunk;
  }
  return true;
}

bool GT911Driver::writeRegister8(uint16_t reg, uint8_t value) {
  return writeRegister(reg, &value, 1);
}

void GT911Driver::clearStatus() {
  if (!writeRegister8(REG_STATUS, 0x00)) {
    DebugLog::println("[GT911] WARNING: status acknowledge failed");
  }
}

bool GT911Driver::readIdentity() {
  uint8_t id[4]{};
  if (!readRegister(REG_PRODUCT_ID, id, sizeof(id))) return false;
  memcpy(productId_, id, 4);
  productId_[4] = '\0';

  uint8_t fw[2]{};
  if (readRegister(REG_FIRMWARE, fw, sizeof(fw))) {
    firmwareVersion_ = static_cast<uint16_t>(fw[0] | (static_cast<uint16_t>(fw[1]) << 8));
  }

  uint8_t resolution[4]{};
  if (readRegister(REG_X_OUTPUT_MAX, resolution, sizeof(resolution))) {
    configuredWidth_ = static_cast<uint16_t>(resolution[0] | (static_cast<uint16_t>(resolution[1]) << 8));
    configuredHeight_ = static_cast<uint16_t>(resolution[2] | (static_cast<uint16_t>(resolution[3]) << 8));
  }

  uint8_t configVersion = 0;
  readRegister(REG_CONFIG_START, &configVersion, 1);

  DebugLog::printf("[GT911] product='%s' firmware=0x%04X configVersion=0x%02X resolution=%ux%u\n",
                productId_, firmwareVersion_, configVersion,
                configuredWidth_, configuredHeight_);
  return true;
}

bool GT911Driver::begin() {
  DebugLog::println("[GT911] own driver begin");
  address_ = 0;

  // Try the documented/common Waveshare address first, then alternative.
  for (const uint8_t candidate : {static_cast<uint8_t>(0x5D), static_cast<uint8_t>(0x14)}) {
    if (selectAddressAndReset(candidate)) {
      address_ = candidate;
      break;
    }
  }

  if (address_ == 0) {
    DebugLog::println("[GT911] ERROR: controller not found after both reset sequences");
    return false;
  }

  if (!readIdentity()) {
    DebugLog::println("[GT911] ERROR: identity registers unreadable");
    address_ = 0;
    return false;
  }

  // Ensure normal operating mode and clear stale touch status.
  writeRegister8(REG_COMMAND, 0x00);
  clearStatus();
  delay(20);

  DebugLog::printf("[GT911] ONLINE address=0x%02X IRQ=%d\n",
                address_, digitalRead(AppConfig::PIN_TOUCH_IRQ));
  return true;
}

bool GT911Driver::poll(GT911Packet &packet) {
  packet = {};
  if (address_ == 0) return false;

  uint8_t status = 0;
  if (!readRegister(REG_STATUS, &status, 1)) return false;

  lastStatus_ = status;
  packet.valid = true;
  packet.status = status;
  packet.newData = (status & 0x80U) != 0;
  packet.pointCount = status & 0x0FU;

  if (!packet.newData) return true;

  if (packet.pointCount > MAX_POINTS) {
    DebugLog::printf("[GT911] invalid point count=%u status=0x%02X\n", packet.pointCount, status);
    clearStatus();
    return true;
  }

  if (packet.pointCount == 0) {
    packet.pressed = false;
    clearStatus();
    return true;
  }

  uint8_t raw[MAX_POINTS * POINT_BYTES]{};
  const size_t bytesToRead = static_cast<size_t>(packet.pointCount) * POINT_BYTES;
  if (!readRegister(REG_FIRST_POINT, raw, bytesToRead)) {
    clearStatus();
    return false;
  }

  for (uint8_t i = 0; i < packet.pointCount; ++i) {
    const size_t o = static_cast<size_t>(i) * POINT_BYTES;
    packet.points[i].id = raw[o + 0];
    packet.points[i].x = static_cast<uint16_t>(raw[o + 1] | (static_cast<uint16_t>(raw[o + 2]) << 8));
    packet.points[i].y = static_cast<uint16_t>(raw[o + 3] | (static_cast<uint16_t>(raw[o + 4]) << 8));
    packet.points[i].size = static_cast<uint16_t>(raw[o + 5] | (static_cast<uint16_t>(raw[o + 6]) << 8));
  }
  packet.pressed = true;

  clearStatus();
  return true;
}
