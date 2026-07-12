#pragma once
#include <Arduino.h>
#include <lvgl.h>
#include "power_supply.h"
#include "touch.h"
#include "debug_log.h"

LV_FONT_DECLARE(ui_font_de_14);
LV_FONT_DECLARE(ui_font_de_16);
LV_FONT_DECLARE(ui_font_de_18);
LV_FONT_DECLARE(ui_font_de_24);

namespace UiInternal {
extern PowerSupplyState *state;
extern lv_obj_t *pages[5];
extern lv_obj_t *nav[5];
extern int currentPage;
extern bool canPaused;
extern bool logPaused;
extern lv_obj_t *clockLabel;

lv_color_t bg(); lv_color_t panelColor(); lv_color_t border();
lv_color_t green(); lv_color_t greenDark(); lv_color_t red(); lv_color_t redDark();
lv_color_t blue(); lv_color_t blueDark(); lv_color_t orange(); lv_color_t cyan();
lv_color_t grey(); lv_color_t lightGrey(); lv_color_t darkGrey();

void stylePanel(lv_obj_t *obj, int radius = 9);
lv_obj_t *makeLabel(lv_obj_t *parent, const char *text,
                    const lv_font_t *font = &ui_font_de_16,
                    lv_color_t color = lv_color_white());
lv_obj_t *makeButton(lv_obj_t *parent, const char *text, int x, int y, int w, int h,
                     lv_color_t color, const lv_font_t *font = &ui_font_de_24);
lv_obj_t *makeLed(lv_obj_t *parent, int x, int y, int size = 16);
void setLed(lv_obj_t *obj, bool active, bool faultStyle = false);
void setFloat(lv_obj_t *obj, float value, uint8_t decimals, const char *unit);
void formatClock(uint32_t ms, char *dest, size_t size);
void showPage(int index);
void navEvent(lv_event_t *event);
void actionEvent(lv_event_t *event);
void presetEvent(lv_event_t *event);

void makeMain(lv_obj_t *parent);
void updateMain(const PowerSupplyState &s, uint32_t now);
void makeScope(lv_obj_t *parent);
void updateScope(const PowerSupplyState &s, uint32_t now);
void handleScopeTouch(const TouchSample &sample);
void makeCan(lv_obj_t *parent);
void updateCan();
void makeLog(lv_obj_t *parent);
void updateLog();
void makeInfo(lv_obj_t *parent);
void updateInfo(const PowerSupplyState &s, uint32_t now);
bool infoCanVisible();
void makeSettings(lv_obj_t *parent);
void updateTouchSettings(const TouchSample &sample);
}
