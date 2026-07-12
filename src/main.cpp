#include <Arduino.h>
#include <lvgl.h>
#include "app_config.h"
#include "debug_log.h"
#include "board_io.h"
#include "display.h"
#include "touch.h"
#include "power_supply.h"
#include "settings.h"
#include "can_driver.h"
#include "ui.h"

PowerSupplyState state;
static TouchSample lastTouch;

static void lvTouchRead(lv_indev_t *, lv_indev_data_t *d) {
  TouchSample s;
  Touch::poll(s);
  lastTouch = s;
  Ui::handleTouch(s);
  d->state = s.pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
  d->point.x = s.x;
  d->point.y = s.y;
}

void setup() {
  DebugLog::begin();
  DebugLog::println("\n===== NETZTEIL-HMI v9.4 =====");
  DebugLog::printf("[DEBUG] UART TX=GPIO%d RX=GPIO%d, %lu Baud\n",
                   AppConfig::PIN_DEBUG_TX, AppConfig::PIN_DEBUG_RX,
                   static_cast<unsigned long>(AppConfig::SERIAL_BAUD));

  Settings::begin(state);
  DebugLog::println("[BOOT] Board/CH422G");
  if (!BoardIO::begin()) DebugLog::println("[BOOT] CH422G FEHLER");
  BoardIO::selectCanMode(); // FSUSB42 SEL=HIGH => HSD2 => CAN
  BoardIO::setBacklight(true);

  DebugLog::println("[BOOT] Display/LVGL");
  if (!Display::begin()) {
    DebugLog::println("[BOOT] DISPLAY FEHLER");
    while (true) delay(1000);
  }

  DebugLog::println("[BOOT] GT911");
  if (!Touch::begin()) DebugLog::println("[BOOT] TOUCH FEHLER");
  lv_indev_t *ind = lv_indev_create();
  lv_indev_set_type(ind, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(ind, lvTouchRead);

  DebugLog::println("[BOOT] CAN");
  state.canStarted = CanDriver::begin();
  state.linkState = state.canStarted ? CanLinkState::Recovering : CanLinkState::Offline;
  Ui::begin(state);
  // STOP is the safe boot state; nevertheless Message 1 must be sent every 1 s as heartbeat.
  CanDriver::sendCommand(state);
  DebugLog::println("[APP] Bereit");
}

void loop() {
  CanDriver::poll(state);
  Settings::task(state);
  const uint32_t now = millis();
  state.updateEnergy(now);
  if (now - state.lastTxMs >= AppConfig::TX_PERIOD_MS) CanDriver::sendCommand(state);

  static uint32_t uiMs = 0;
  if (now - uiMs >= 100) {
    uiMs = now;
    Ui::update(state);
    Ui::updateTouchDiag(lastTouch);
  }

  static uint32_t healthMs = 0;
  if (now - healthMs >= 5000) {
    healthMs = now;
    DebugLog::printf("[HEALTH] heap=%u psram=%u touch=%s CAN=%s RX=%s age=%lu ms status=0x%02X\n",
                     ESP.getFreeHeap(), ESP.getFreePsram(), Touch::online()?"OK":"FEHLER",
                     state.canStarted?"OK":"FEHLER", state.online(now)?"OK":"TIMEOUT",
                     state.lastRxMs ? static_cast<unsigned long>(now-state.lastRxMs) : 0UL,
                     state.status);
  }
  Display::task();
  delay(3);
}
