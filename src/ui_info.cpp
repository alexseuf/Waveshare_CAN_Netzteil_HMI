#include "ui_internal.h"
#include "app_config.h"
#include <ESP.h>

namespace UiInternal {
namespace {
lv_obj_t *infoText = nullptr;
lv_obj_t *infoSystem = nullptr;
lv_obj_t *infoCan = nullptr;
bool showCanLogger = false;

void infoTab(lv_event_t *event) {
  showCanLogger =
      static_cast<int>(reinterpret_cast<intptr_t>(lv_event_get_user_data(event))) == 1;

  if (showCanLogger) {
    lv_obj_add_flag(infoSystem, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(infoCan, LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_clear_flag(infoSystem, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(infoCan, LV_OBJ_FLAG_HIDDEN);
  }
}
}  // namespace

void makeInfo(lv_obj_t *parent) {
  lv_obj_t *box = lv_obj_create(parent);
  lv_obj_set_size(box, 780, 422);
  stylePanel(box);

  makeLabel(box, "INFO / DIAGNOSE", &lv_font_montserrat_24, blue());

  lv_obj_t *systemButton =
      makeButton(box, "SYSTEM", 300, 3, 130, 36, blue(), &lv_font_montserrat_16);
  lv_obj_add_event_cb(systemButton, infoTab, LV_EVENT_CLICKED,
                      reinterpret_cast<void *>(0));

  lv_obj_t *canButton =
      makeButton(box, "CAN-LOGGER", 440, 3, 150, 36, blue(),
                 &lv_font_montserrat_16);
  lv_obj_add_event_cb(canButton, infoTab, LV_EVENT_CLICKED,
                      reinterpret_cast<void *>(1));

  infoSystem = lv_obj_create(box);
  lv_obj_remove_style_all(infoSystem);
  lv_obj_set_pos(infoSystem, 6, 50);
  lv_obj_set_size(infoSystem, 748, 344);

  infoText = makeLabel(infoSystem, "", &ui_font_de_16);
  lv_obj_set_pos(infoText, 12, 8);

  infoCan = lv_obj_create(box);
  lv_obj_remove_style_all(infoCan);
  lv_obj_set_pos(infoCan, 6, 50);
  lv_obj_set_size(infoCan, 748, 344);
  makeCan(infoCan);
  lv_obj_add_flag(infoCan, LV_OBJ_FLAG_HIDDEN);
}

bool infoCanVisible() { return showCanLogger; }

void updateInfo(const PowerSupplyState &s, uint32_t now) {
  if (!infoText) return;

  char text[760];
  snprintf(text, sizeof(text),
           "Firmware: %s\n"
           "Heap frei: %u Byte\n"
           "PSRAM frei: %u Byte\n"
           "CAN-Treiber: %s / 250 kbit/s\n"
           "Link-State: %s\n"
           "TX-Zyklus: 1000 ms\n"
           "Letztes RX: %lu ms\n"
           "Statusbyte: 0x%02X\n"
           "Energie: %.3f kWh\n"
           "TX-ID: 0x%08lX\n"
           "RX-ID: 0x%08lX",
           AppConfig::FIRMWARE_VERSION,
           ESP.getFreeHeap(),
           ESP.getFreePsram(),
           s.canStarted ? "gestartet" : "Fehler",
           s.linkStateText(),
           s.lastRxMs ? static_cast<unsigned long>(now - s.lastRxMs) : 0UL,
           static_cast<unsigned>(s.status),
           s.energyKWh,
           static_cast<unsigned long>(AppConfig::CAN_ID_COMMAND),
           static_cast<unsigned long>(AppConfig::CAN_ID_STATUS));

  lv_label_set_text(infoText, text);
}

}  // namespace UiInternal
