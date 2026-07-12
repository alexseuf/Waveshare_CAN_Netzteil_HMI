#pragma once
#include <Arduino.h>

struct GT911Point {
  uint8_t id = 0;
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t size = 0;
};

struct GT911Packet {
  bool valid = false;
  bool newData = false;
  bool pressed = false;
  uint8_t status = 0;
  uint8_t pointCount = 0;
  GT911Point points[5]{};
};

class GT911Driver {
public:
  bool begin();
  bool poll(GT911Packet &packet);
  bool online() const { return address_ != 0; }
  uint8_t address() const { return address_; }
  const char *productId() const { return productId_; }
  uint16_t firmwareVersion() const { return firmwareVersion_; }
  uint16_t configuredWidth() const { return configuredWidth_; }
  uint16_t configuredHeight() const { return configuredHeight_; }
  uint32_t readErrors() const { return readErrors_; }
  uint32_t writeErrors() const { return writeErrors_; }
  uint8_t lastStatus() const { return lastStatus_; }

private:
  bool selectAddressAndReset(uint8_t address);
  bool probe(uint8_t address);
  bool readRegister(uint16_t reg, uint8_t *data, size_t length);
  bool writeRegister(uint16_t reg, const uint8_t *data, size_t length);
  bool writeRegister8(uint16_t reg, uint8_t value);
  bool readIdentity();
  void clearStatus();

  uint8_t address_ = 0;
  char productId_[5] = "----";
  uint16_t firmwareVersion_ = 0;
  uint16_t configuredWidth_ = 0;
  uint16_t configuredHeight_ = 0;
  uint32_t readErrors_ = 0;
  uint32_t writeErrors_ = 0;
  uint8_t lastStatus_ = 0;
};
