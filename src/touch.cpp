#include "touch.h"
#include "app_config.h"
#include <Arduino.h>
#include "debug_log.h"

namespace {
GT911Driver driver;
bool heldPressed = false;
uint16_t heldRawX = 0;
uint16_t heldRawY = 0;
uint16_t heldX = 0;
uint16_t heldY = 0;
uint8_t heldPoints = 0;
uint16_t heldPointX[5]{};
uint16_t heldPointY[5]{};

void transform(uint16_t rawX, uint16_t rawY, uint16_t &screenX, uint16_t &screenY) {
  int32_t x = rawX;
  int32_t y = rawY;
  if (AppConfig::TOUCH_SWAP_XY) {
    const int32_t temp = x;
    x = y;
    y = temp;
  }

  x = constrain(x, 0, AppConfig::LCD_WIDTH - 1);
  y = constrain(y, 0, AppConfig::LCD_HEIGHT - 1);
  if (AppConfig::TOUCH_INVERT_X) x = AppConfig::LCD_WIDTH - 1 - x;
  if (AppConfig::TOUCH_INVERT_Y) y = AppConfig::LCD_HEIGHT - 1 - y;
  screenX = static_cast<uint16_t>(x);
  screenY = static_cast<uint16_t>(y);
}
}

bool Touch::begin() {
  heldPressed = false;
  const bool ok = driver.begin();
  if (ok) {
    DebugLog::printf("[TOUCH] transform swapXY=%d invertX=%d invertY=%d\n",
                  AppConfig::TOUCH_SWAP_XY, AppConfig::TOUCH_INVERT_X,
                  AppConfig::TOUCH_INVERT_Y);
  }
  return ok;
}

bool Touch::poll(TouchSample &sample) {
  sample = {};
  GT911Packet packet;
  if (!driver.poll(packet)) return false;

  sample.valid = packet.valid;
  sample.newPacket = packet.newData;
  sample.status = packet.status;
  sample.points = packet.pointCount;

  if (!packet.newData) {
    sample.pressed = heldPressed;
    sample.rawX = heldRawX;
    sample.rawY = heldRawY;
    sample.x = heldX;
    sample.y = heldY;
    sample.points = heldPoints;
    for(uint8_t i=0;i<heldPoints && i<5;i++){ sample.pointX[i]=heldPointX[i]; sample.pointY[i]=heldPointY[i]; }
    return true;
  }

  if (!packet.pressed || packet.pointCount == 0) {
    heldPressed = false; heldPoints = 0;
    sample.pressed = false;
    return true;
  }

  sample.rawX = packet.points[0].x;
  sample.rawY = packet.points[0].y;
  transform(sample.rawX, sample.rawY, sample.x, sample.y);
  for(uint8_t i=0;i<packet.pointCount && i<5;i++) transform(packet.points[i].x, packet.points[i].y, sample.pointX[i], sample.pointY[i]);
  sample.pressed = true;

  heldPressed = true; heldPoints = packet.pointCount;
  for(uint8_t i=0;i<heldPoints && i<5;i++){ heldPointX[i]=sample.pointX[i]; heldPointY[i]=sample.pointY[i]; }
  heldRawX = sample.rawX;
  heldRawY = sample.rawY;
  heldX = sample.x;
  heldY = sample.y;
  return true;
}

bool Touch::online() { return driver.online(); }
uint8_t Touch::address() { return driver.address(); }
const char *Touch::productId() { return driver.productId(); }
uint16_t Touch::configuredWidth() { return driver.configuredWidth(); }
uint16_t Touch::configuredHeight() { return driver.configuredHeight(); }
uint32_t Touch::readErrors() { return driver.readErrors(); }
uint32_t Touch::writeErrors() { return driver.writeErrors(); }
