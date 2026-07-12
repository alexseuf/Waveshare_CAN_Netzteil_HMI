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
#include "hmi_wifi.h"
#include "ui.h"
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

PowerSupplyState state;
static TouchSample lastTouch;
static volatile bool networkStarted = false;
static TaskHandle_t networkTaskHandle = nullptr;

namespace {
constexpr BaseType_t NETWORK_CORE = 0;
constexpr UBaseType_t NETWORK_PRIORITY = 1;
constexpr uint32_t NETWORK_STACK_BYTES = 8192;

void networkTask(void *) {
  DebugLog::printf("[NET-TASK] gestartet auf Core %d, Startverzögerung 3000 ms\n",
                   xPortGetCoreID());
  vTaskDelay(pdMS_TO_TICKS(3000));

  DebugLog::println("[BOOT] WiFi/NTP jetzt im Netzwerk-Task starten");
  HmiWifi::begin();
  networkStarted = true;
  DebugLog::println("[BOOT] WiFi/NTP Netzwerk-Task bereit");

  TickType_t lastWake = xTaskGetTickCount();
  for (;;) {
    HmiWifi::task();
    vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(10));
  }
}
}

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
  DebugLog::printf("\n===== NETZTEIL-HMI v%s =====\n", AppConfig::FIRMWARE_VERSION);
  DebugLog::printf("[DEBUG] UART TX=GPIO%d RX=GPIO%d, %lu Baud\n",
                   AppConfig::PIN_DEBUG_TX, AppConfig::PIN_DEBUG_RX,
                   static_cast<unsigned long>(AppConfig::SERIAL_BAUD));
  DebugLog::printf("[BOOT] Arduino setup/loop auf Core %d\n", xPortGetCoreID());

  Settings::begin(state);
  DebugLog::println("[BOOT] Board/CH422G");
  if (!BoardIO::begin()) DebugLog::println("[BOOT] CH422G FEHLER");
  BoardIO::selectCanMode();
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

  DebugLog::println("[BOOT] UI");
  Ui::begin(state);
  CanDriver::sendCommand(state);
  DebugLog::println("[APP] Basisfunktionen bereit");

  const BaseType_t created = xTaskCreatePinnedToCore(
      networkTask,
      "network",
      NETWORK_STACK_BYTES,
      nullptr,
      NETWORK_PRIORITY,
      &networkTaskHandle,
      NETWORK_CORE);

  if (created == pdPASS) {
    DebugLog::printf("[BOOT] Netzwerk-Task auf Core %d angelegt, Stack=%lu Byte\n",
                     NETWORK_CORE, static_cast<unsigned long>(NETWORK_STACK_BYTES));
  } else {
    networkTaskHandle = nullptr;
    DebugLog::println("[BOOT] FEHLER: Netzwerk-Task konnte nicht angelegt werden");
  }
}

void loop() {
  const uint32_t loopStartUs = micros();

  CanDriver::poll(state);
  Settings::task(state);

  // Nur der Start des WiFi-Scans läuft auf dem Arduino-/UI-Core.
  // Der restliche Netzwerkbetrieb bleibt auf Core 0.
  if (networkStarted) HmiWifi::serviceScanOnUiCore();

  const uint32_t now = millis();
  state.updateEnergy(now);

  if (now - state.lastTxMs >= AppConfig::TX_PERIOD_MS) {
    CanDriver::sendCommand(state);
  }

  static uint32_t uiMs = 0;
  if (now - uiMs >= 100) {
    uiMs = now;
    Ui::update(state);
    Ui::updateTouchDiag(lastTouch);
  }

  static uint32_t healthMs = 0;
  if (now - healthMs >= 5000) {
    healthMs = now;
    const String wifiState = networkStarted
                                 ? HmiWifi::statusText()
                                 : String("noch nicht gestartet");
    const size_t intFree = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
    const size_t intLargest = heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL);
    const size_t psTotal = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
    const size_t psFree = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    const size_t psLargest = heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM);
    const UBaseType_t netStackFree = networkTaskHandle
                                         ? uxTaskGetStackHighWaterMark(networkTaskHandle)
                                         : 0;

    DebugLog::printf(
        "[HEALTH] core=%d heap=%u intFree=%u intLargest=%u psTotal=%u "
        "psFree=%u psLargest=%u netStackFree=%u touch=%s CAN=%s RX=%s "
        "age=%lu ms status=0x%02X WiFi=%s\n",
        xPortGetCoreID(), ESP.getFreeHeap(), static_cast<unsigned>(intFree),
        static_cast<unsigned>(intLargest), static_cast<unsigned>(psTotal),
        static_cast<unsigned>(psFree), static_cast<unsigned>(psLargest),
        static_cast<unsigned>(netStackFree), Touch::online() ? "OK" : "FEHLER",
        state.canStarted ? "OK" : "FEHLER", state.online(now) ? "OK" : "TIMEOUT",
        state.lastRxMs ? static_cast<unsigned long>(now - state.lastRxMs) : 0UL,
        state.status, wifiState.c_str());
  }

  Display::task();

  const uint32_t busyUs = micros() - loopStartUs;
  static uint64_t accumulatedBusyUs = 0;
  static uint32_t performanceWindowUs = micros();
  accumulatedBusyUs += busyUs;
  const uint32_t performanceNowUs = micros();
  const uint32_t elapsedUs = performanceNowUs - performanceWindowUs;
  if (elapsedUs >= 1000000UL) {
    uint64_t loadValue = (accumulatedBusyUs * 100ULL) / elapsedUs;
    if (loadValue > 100ULL) loadValue = 100ULL;
    Ui::setPerformance(static_cast<uint8_t>(loadValue), busyUs);
    accumulatedBusyUs = 0;
    performanceWindowUs = performanceNowUs;
  }

  delay(3);
}