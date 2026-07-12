#pragma once
#include <Arduino.h>
#include "gt911_driver.h"

struct TouchSample {
  bool valid = false;
  bool newPacket = false;
  bool pressed = false;
  uint8_t points = 0;
  uint8_t status = 0;
  uint16_t rawX = 0;
  uint16_t rawY = 0;
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t pointX[5]{};
  uint16_t pointY[5]{};
};

namespace Touch {
bool begin();
bool poll(TouchSample &sample);
bool online();
uint8_t address();
const char *productId();
uint16_t configuredWidth();
uint16_t configuredHeight();
uint32_t readErrors();
uint32_t writeErrors();
}
