#include "ui_internal.h"
#include "app_config.h"
#include "can_driver.h"
#include "display.h"
#include "hmi_wifi.h"
#include <ESP.h>
#include <esp_system.h>
#include <cstring>

namespace UiInternal {
namespace {
lv_obj_t *infoText = nullptr;
lv_obj_t *infoSystem = nullptr;
lv_obj_t *infoCan = nullptr;
lv_obj_t *settingsOverview = nullptr;
lv_obj_t *debugPage = nullptr;
lv_obj_t *wifiPage = nullptr;
lv_obj_t *mqttPage = nullptr;
lv_obj_t *overviewMetrics = nullptr;
lv_obj_t *debugMetrics = nullptr;
lv_obj_t *wifiStatus = nullptr;
lv_obj_t *wifiDropdown = nullptr;
lv_obj_t *wifiPassword = nullptr;
lv_obj_t *wifiPasswordCount = nullptr;
lv_obj_t *wifiScanButton = nullptr;
lv_obj_t *wifiKeyboard = nullptr;
lv_obj_t *wifiEyeButton = nullptr;
bool showCanLogger = false;
bool wifiOptionsDirty = true;
bool passwordVisible = false;

constexpr uint32_t TEXT_BRIGHT = 0xE8EEF4;
constexpr uint32_t CHECK_BG = 0x182431;
constexpr uint32_t CHECK_BORDER = 0x8EA0B0;

enum class SettingsPage : uint8_t { Overview, Debug, Wifi, Mqtt };
SettingsPage settingsPage = SettingsPage::Overview;

static const char *germanLowerMap[] = {
    "q", "w", "e", "r", "t", "z", "u", "i", "o", "p", "ü", LV_SYMBOL_BACKSPACE, "\n",
    "ABC", "a", "s", "d", "f", "g", "h", "j", "k", "l", "ö", "ä", "\n",
    "123", "y", "x", "c", "v", "b", "n", "m", "ß", "-", "_", "\n",
    ".com", "@", " ", ".", LV_SYMBOL_NEW_LINE, ""
};

static const char *germanUpperMap[] = {
    "Q", "W", "E", "R", "T", "Z", "U", "I", "O", "P", "Ü", LV_SYMBOL_BACKSPACE, "\n",
    "abc", "A", "S", "D", "F", "G", "H", "J", "K", "L", "Ö", "Ä", "\n",
    "123", "Y", "X", "C", "V", "B", "N", "M", "ẞ", "-", "_", "\n",
    ".com", "@", " ", ".", LV_SYMBOL_NEW_LINE, ""
};

void infoTab(lv_event_t *event) {
  showCanLogger = static_cast<int>(reinterpret_cast<intptr_t>(lv_event_get_user_data(event))) == 1;
  if (showCanLogger) {
    lv_obj_add_flag(infoSystem, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(infoCan, LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_clear_flag(infoSystem, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(infoCan, LV_OBJ_FLAG_HIDDEN);
  }
}

void logOption(lv_event_t *event) {
  lv_obj_t *checkbox = static_cast<lv_obj_t *>(lv_event_get_target(event));
  const uint16_t category = static_cast<uint16_t>(
      reinterpret_cast<uintptr_t>(lv_event_get_user_data(event)));
  uint16_t mask = DebugLog::categoryMask();
  if (lv_obj_has_state(checkbox, LV_STATE_CHECKED)) mask |= category;
  else mask &= static_cast<uint16_t>(~category);
  DebugLog::setCategoryMask(mask);
}

void outputOption(lv_event_t *event) {
  lv_obj_t *checkbox = static_cast<lv_obj_t *>(lv_event_get_target(event));
  const int id = static_cast<int>(reinterpret_cast<intptr_t>(lv_event_get_user_data(event)));
  const bool enabled = lv_obj_has_state(checkbox, LV_STATE_CHECKED);
  if (id == 1) DebugLog::setDisplayEnabled(enabled);
  else DebugLog::setUartEnabled(enabled);
}

void styleCheckbox(lv_obj_t *checkbox) {
  const lv_style_selector_t indicator = static_cast<lv_style_selector_t>(LV_PART_INDICATOR);
  const lv_style_selector_t checkedIndicator =
      static_cast<lv_style_selector_t>(LV_PART_INDICATOR | LV_STATE_CHECKED);
  const lv_style_selector_t checkedMain =
      static_cast<lv_style_selector_t>(LV_PART_MAIN | LV_STATE_CHECKED);

  lv_obj_set_style_text_font(checkbox, &ui_font_de_14, LV_PART_MAIN);
  lv_obj_set_style_text_color(checkbox, lv_color_hex(TEXT_BRIGHT), LV_PART_MAIN);
  lv_obj_set_style_text_opa(checkbox, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_set_style_bg_color(checkbox, lv_color_hex(CHECK_BG), indicator);
  lv_obj_set_style_bg_opa(checkbox, LV_OPA_COVER, indicator);
  lv_obj_set_style_border_color(checkbox, lv_color_hex(CHECK_BORDER), indicator);
  lv_obj_set_style_border_width(checkbox, 2, indicator);
  lv_obj_set_style_radius(checkbox, 4, indicator);
  lv_obj_set_style_bg_color(checkbox, blue(), checkedIndicator);
  lv_obj_set_style_border_color(checkbox, lv_color_lighten(blue(), 35), checkedIndicator);
  lv_obj_set_style_text_color(checkbox, lv_color_white(), checkedMain);
}

lv_obj_t *option(lv_obj_t *parent, const char *text, int x, int y, uint16_t category) {
  lv_obj_t *checkbox = lv_checkbox_create(parent);
  lv_checkbox_set_text(checkbox, text);
  lv_obj_set_pos(checkbox, x, y);
  styleCheckbox(checkbox);
  if (DebugLog::categoryEnabled(category)) lv_obj_add_state(checkbox, LV_STATE_CHECKED);
  lv_obj_add_event_cb(checkbox, logOption, LV_EVENT_VALUE_CHANGED,
                      reinterpret_cast<void *>(static_cast<uintptr_t>(category)));
  return checkbox;
}

void hideKeyboard() {
  if (wifiKeyboard) lv_obj_add_flag(wifiKeyboard, LV_OBJ_FLAG_HIDDEN);
}

void showSettingsPage(SettingsPage page) {
  settingsPage = page;
  hideKeyboard();
  lv_obj_t *pagesToHide[] = {settingsOverview, debugPage, wifiPage, mqttPage};
  for (lv_obj_t *pageObject : pagesToHide) {
    if (pageObject) lv_obj_add_flag(pageObject, LV_OBJ_FLAG_HIDDEN);
  }

  lv_obj_t *target = settingsOverview;
  if (page == SettingsPage::Debug) target = debugPage;
  else if (page == SettingsPage::Wifi) target = wifiPage;
  else if (page == SettingsPage::Mqtt) target = mqttPage;
  if (target) lv_obj_clear_flag(target, LV_OBJ_FLAG_HIDDEN);
}

void settingsNav(lv_event_t *event) {
  showSettingsPage(static_cast<SettingsPage>(
      reinterpret_cast<intptr_t>(lv_event_get_user_data(event))));
}

lv_obj_t *makeIconTitle(lv_obj_t *parent, const char *icon, const char *text,
                        int y, lv_color_t color) {
  lv_obj_t *group = lv_obj_create(parent);
  lv_obj_remove_style_all(group);
  lv_obj_set_size(group, 450, 38);
  lv_obj_set_pos(group, 165, y);
  lv_obj_clear_flag(group, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_t *iconLabel = makeLabel(group, icon, &lv_font_montserrat_24, color);
  lv_obj_set_pos(iconLabel, 0, 4);
  lv_obj_t *textLabel = makeLabel(group, text, &ui_font_de_24, color);
  lv_obj_set_pos(textLabel, 42, 0);
  return group;
}

lv_obj_t *card(lv_obj_t *parent, const char *icon, const char *title,
               const char *description, int x, lv_color_t accent,
               SettingsPage target) {
  lv_obj_t *button = lv_button_create(parent);
  lv_obj_remove_style_all(button);
  lv_obj_set_pos(button, x, 54);
  lv_obj_set_size(button, 230, 222);
  lv_obj_add_flag(button, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_bg_color(button, lv_color_hex(0x0D1823), 0);
  lv_obj_set_style_bg_opa(button, LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(button, border(), 0);
  lv_obj_set_style_border_width(button, 2, 0);
  lv_obj_set_style_radius(button, 12, 0);
  lv_obj_set_style_bg_color(button, lv_color_hex(0x14283A), LV_STATE_PRESSED);

  lv_obj_t *iconLabel = makeLabel(button, icon, &lv_font_montserrat_24, accent);
  lv_obj_align(iconLabel, LV_ALIGN_TOP_MID, 0, 18);
  lv_obj_t *heading = makeLabel(button, title, &ui_font_de_18, lv_color_white());
  lv_obj_align(heading, LV_ALIGN_TOP_MID, 0, 62);
  lv_obj_t *body = makeLabel(button, description, &ui_font_de_14,
                             lv_color_hex(TEXT_BRIGHT));
  lv_label_set_long_mode(body, LV_LABEL_LONG_WRAP);
  lv_obj_set_width(body, 198);
  lv_obj_set_style_text_align(body, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(body, LV_ALIGN_TOP_MID, 0, 102);
  lv_obj_t *arrow = makeLabel(button, LV_SYMBOL_RIGHT, &lv_font_montserrat_18,
                              lv_color_white());
  lv_obj_align(arrow, LV_ALIGN_BOTTOM_RIGHT, -14, -12);
  lv_obj_add_event_cb(button, settingsNav, LV_EVENT_CLICKED,
                      reinterpret_cast<void *>(static_cast<intptr_t>(target)));
  return button;
}

lv_obj_t *backButton(lv_obj_t *parent) {
  lv_obj_t *button = lv_button_create(parent);
  lv_obj_remove_style_all(button);
  lv_obj_set_pos(button, 8, 5);
  lv_obj_set_size(button, 132, 36);
  lv_obj_add_flag(button, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_bg_color(button, darkGrey(), LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(button, lv_color_darken(darkGrey(), 25), LV_STATE_PRESSED);
  lv_obj_set_style_bg_opa(button, LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(button, border(), 0);
  lv_obj_set_style_border_width(button, 2, 0);
  lv_obj_set_style_radius(button, 9, 0);
  lv_obj_t *icon = makeLabel(button, LV_SYMBOL_LEFT, &lv_font_montserrat_16,
                             lv_color_white());
  lv_obj_set_pos(icon, 14, 9);
  lv_obj_t *text = makeLabel(button, "ZURÜCK", &ui_font_de_14, lv_color_white());
  lv_obj_set_pos(text, 40, 8);
  lv_obj_add_event_cb(button, settingsNav, LV_EVENT_CLICKED,
                      reinterpret_cast<void *>(
                          static_cast<intptr_t>(SettingsPage::Overview)));
  return button;
}

const char *resetReasonText() {
  switch (esp_reset_reason()) {
    case ESP_RST_POWERON: return "Power On";
    case ESP_RST_SW: return "Software";
    case ESP_RST_PANIC: return "Panic";
    case ESP_RST_INT_WDT: return "Interrupt WDT";
    case ESP_RST_TASK_WDT: return "Task WDT";
    case ESP_RST_WDT: return "Watchdog";
    case ESP_RST_DEEPSLEEP: return "Deep Sleep";
    case ESP_RST_BROWNOUT: return "Brownout";
    default: return "Unbekannt";
  }
}

void wifiScanEvent(lv_event_t *) {
  HmiWifi::requestScan();
  wifiOptionsDirty = true;
}

void wifiConnectEvent(lv_event_t *) {
  char selected[64] = {0};
  lv_dropdown_get_selected_str(wifiDropdown, selected, sizeof(selected));
  String selectedSsid = selected;
  const int marker = selectedSsid.lastIndexOf("  (");
  if (marker > 0) selectedSsid = selectedSsid.substring(0, marker);
  HmiWifi::connectTo(selectedSsid, String(lv_textarea_get_text(wifiPassword)));
  hideKeyboard();
}

void wifiDisconnectEvent(lv_event_t *) { HmiWifi::disconnect(); }

void wifiApEvent(lv_event_t *) {
  if (HmiWifi::accessPointActive()) HmiWifi::stopAccessPoint();
  else HmiWifi::startAccessPoint();
}

void updatePasswordCount() {
  if (!wifiPasswordCount || !wifiPassword) return;
  char text[32];
  snprintf(text, sizeof(text), "%u Zeichen",
           static_cast<unsigned>(std::strlen(lv_textarea_get_text(wifiPassword))));
  lv_label_set_text(wifiPasswordCount, text);
}

void passwordChangedEvent(lv_event_t *) { updatePasswordCount(); }

void passwordEyeEvent(lv_event_t *) {
  passwordVisible = !passwordVisible;
  lv_textarea_set_password_mode(wifiPassword, !passwordVisible);
  lv_obj_set_style_bg_color(wifiEyeButton,
                            passwordVisible ? greenDark() : darkGrey(), 0);
}

void passwordFocusEvent(lv_event_t *) {
  if (!wifiKeyboard) return;
  lv_keyboard_set_textarea(wifiKeyboard, wifiPassword);
  lv_obj_clear_flag(wifiKeyboard, LV_OBJ_FLAG_HIDDEN);
  lv_obj_move_foreground(wifiKeyboard);
  updatePasswordCount();
}

void keyboardEvent(lv_event_t *event) {
  const lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) hideKeyboard();
}

void rebuildWifiOptions() {
  if (!wifiDropdown || !HmiWifi::scanReady()) return;
  String options;
  const int count = HmiWifi::scanCount();
  for (int i = 0; i < count && i < 20; ++i) {
    const String name = HmiWifi::scanSsid(i);
    if (name.isEmpty()) continue;
    if (options.length()) options += '\n';
    options += name;
    options += "  (";
    options += String(HmiWifi::scanRssi(i));
    options += " dBm";
    if (HmiWifi::scanEncrypted(i)) options += ", gesichert";
    options += ")";
  }
  if (options.isEmpty()) options = "Keine Netzwerke gefunden";
  lv_dropdown_set_options(wifiDropdown, options.c_str());
  wifiOptionsDirty = false;
}
}  // namespace

void makeInfo(lv_obj_t *parent) {
  lv_obj_t *box = lv_obj_create(parent);
  lv_obj_set_size(box, 780, 422);
  stylePanel(box);
  makeLabel(box, "INFO / DIAGNOSE", &lv_font_montserrat_24, blue());

  lv_obj_t *systemButton = makeButton(box, "SYSTEM", 300, 3, 130, 36, blue(),
                                      &lv_font_montserrat_16);
  lv_obj_add_event_cb(systemButton, infoTab, LV_EVENT_CLICKED,
                      reinterpret_cast<void *>(0));
  lv_obj_t *canButton = makeButton(box, "CAN-LOGGER", 440, 3, 150, 36, blue(),
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

void updateInfo(const PowerSupplyState &stateData, uint32_t now) {
  if (!infoText) return;
  char text[760];
  snprintf(text, sizeof(text),
           "Firmware: %s\nHeap frei: %u Byte\nPSRAM frei: %u Byte\n"
           "CAN-Treiber: %s / 250 kbit/s\nLink-State: %s\nTX-Zyklus: 1000 ms\n"
           "Letztes RX: %lu ms\nStatusbyte: 0x%02X\nEnergie: %.3f kWh\n"
           "TX-ID: 0x%08lX\nRX-ID: 0x%08lX",
           AppConfig::FIRMWARE_VERSION, ESP.getFreeHeap(), ESP.getFreePsram(),
           stateData.canStarted ? "gestartet" : "Fehler", stateData.linkStateText(),
           stateData.lastRxMs ? now - stateData.lastRxMs : 0,
           static_cast<unsigned>(stateData.status), stateData.energyKWh,
           static_cast<unsigned long>(AppConfig::CAN_ID_COMMAND),
           static_cast<unsigned long>(AppConfig::CAN_ID_STATUS));
  lv_label_set_text(infoText, text);
}

void makeSettings(lv_obj_t *parent) {
  lv_obj_t *box = lv_obj_create(parent);
  lv_obj_set_size(box, 780, 422);
  stylePanel(box);

  settingsOverview = lv_obj_create(box);
  lv_obj_remove_style_all(settingsOverview);
  lv_obj_set_size(settingsOverview, 760, 402);
  lv_obj_set_pos(settingsOverview, 0, 0);
  lv_obj_clear_flag(settingsOverview, LV_OBJ_FLAG_SCROLLABLE);
  makeIconTitle(settingsOverview, LV_SYMBOL_SETTINGS, "EINSTELLUNGEN", 5,
                lv_color_white());
  card(settingsOverview, LV_SYMBOL_EDIT, "Debug Settings",
       "Logs, Diagnose, Anzeige und Entwickleroptionen konfigurieren.",
       8, blue(), SettingsPage::Debug);
  card(settingsOverview, LV_SYMBOL_WIFI, "WiFi",
       "WLAN-Verbindung konfigurieren, Netzwerkstatus und NTP-Zeit anzeigen.",
       265, green(), SettingsPage::Wifi);
  card(settingsOverview, LV_SYMBOL_UPLOAD, "MQTT / Home Assistant",
       "MQTT-Broker und Home-Assistant-Integration konfigurieren.",
       522, lv_color_hex(0xA060FF), SettingsPage::Mqtt);

  overviewMetrics = makeLabel(settingsOverview, "", &ui_font_de_14,
                              lv_color_hex(TEXT_BRIGHT));
  lv_obj_set_pos(overviewMetrics, 15, 302);
  lv_obj_set_size(overviewMetrics, 730, 70);
  lv_obj_set_style_bg_color(overviewMetrics, lv_color_hex(0x0D1823), 0);
  lv_obj_set_style_bg_opa(overviewMetrics, LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(overviewMetrics, border(), 0);
  lv_obj_set_style_border_width(overviewMetrics, 1, 0);
  lv_obj_set_style_radius(overviewMetrics, 10, 0);
  lv_obj_set_style_pad_all(overviewMetrics, 14, 0);

  debugPage = lv_obj_create(box);
  lv_obj_remove_style_all(debugPage);
  lv_obj_set_size(debugPage, 760, 402);
  lv_obj_set_pos(debugPage, 0, 0);
  backButton(debugPage);
  makeIconTitle(debugPage, LV_SYMBOL_EDIT, "DEBUG-EINSTELLUNGEN", 6, blue());

  lv_obj_t *debugLeft = lv_obj_create(debugPage);
  lv_obj_set_pos(debugLeft, 8, 52);
  lv_obj_set_size(debugLeft, 354, 330);
  stylePanel(debugLeft, 10);
  makeLabel(debugLeft, "Einstellungen", &ui_font_de_18, lv_color_white());
  option(debugLeft, "System", 12, 42, DebugLog::SYSTEM);
  option(debugLeft, "Fehler", 12, 72, DebugLog::ERROR);
  option(debugLeft, "Warnungen", 12, 102, DebugLog::WARNING);
  option(debugLeft, "CAN RX", 170, 42, DebugLog::CAN_RX);
  option(debugLeft, "CAN TX", 170, 72, DebugLog::CAN_TX);
  option(debugLeft, "Bedienung", 170, 102, DebugLog::UI);
  option(debugLeft, "Touch", 12, 132, DebugLog::TOUCH);
  option(debugLeft, "NVS / Speicher", 170, 132, DebugLog::STORAGE);

  lv_obj_t *displayOutput = lv_checkbox_create(debugLeft);
  lv_checkbox_set_text(displayOutput, "Ausgabe auf Display");
  lv_obj_set_pos(displayOutput, 12, 175);
  styleCheckbox(displayOutput);
  if (DebugLog::displayEnabled()) lv_obj_add_state(displayOutput, LV_STATE_CHECKED);
  lv_obj_add_event_cb(displayOutput, outputOption, LV_EVENT_VALUE_CHANGED,
                      reinterpret_cast<void *>(1));

  lv_obj_t *uartOutput = lv_checkbox_create(debugLeft);
  lv_checkbox_set_text(uartOutput, "Ausgabe über UART");
  lv_obj_set_pos(uartOutput, 12, 208);
  styleCheckbox(uartOutput);
  if (DebugLog::uartEnabled()) lv_obj_add_state(uartOutput, LV_STATE_CHECKED);
  lv_obj_add_event_cb(uartOutput, outputOption, LV_EVENT_VALUE_CHANGED,
                      reinterpret_cast<void *>(2));

  lv_obj_t *energyReset = makeButton(debugLeft, "ENERGIE LÖSCHEN", 12, 254,
                                     150, 45, blue(), &ui_font_de_14);
  lv_obj_add_event_cb(energyReset, actionEvent, LV_EVENT_CLICKED,
                      reinterpret_cast<void *>(21));
  lv_obj_t *factoryReset = makeButton(debugLeft, "WERKSEINST.", 176, 254,
                                      150, 45, redDark(), &ui_font_de_14);
  lv_obj_add_event_cb(factoryReset, actionEvent, LV_EVENT_CLICKED,
                      reinterpret_cast<void *>(20));

  lv_obj_t *debugRight = lv_obj_create(debugPage);
  lv_obj_set_pos(debugRight, 378, 52);
  lv_obj_set_size(debugRight, 374, 330);
  stylePanel(debugRight, 10);
  makeLabel(debugRight, "Systemdiagnose", &ui_font_de_18, lv_color_white());
  debugMetrics = makeLabel(debugRight, "", &ui_font_de_16,
                           lv_color_hex(TEXT_BRIGHT));
  lv_obj_set_pos(debugMetrics, 12, 42);
  lv_obj_set_size(debugMetrics, 340, 250);

  wifiPage = lv_obj_create(box);
  lv_obj_remove_style_all(wifiPage);
  lv_obj_set_size(wifiPage, 760, 402);
  lv_obj_set_pos(wifiPage, 0, 0);
  backButton(wifiPage);
  makeIconTitle(wifiPage, LV_SYMBOL_WIFI, "WIFI", 6, green());

  lv_obj_t *statusPanel = lv_obj_create(wifiPage);
  lv_obj_set_pos(statusPanel, 8, 52);
  lv_obj_set_size(statusPanel, 300, 300);
  stylePanel(statusPanel, 10);
  makeLabel(statusPanel, "Status", &ui_font_de_18, lv_color_white());
  wifiStatus = makeLabel(statusPanel, "", &ui_font_de_14,
                         lv_color_hex(TEXT_BRIGHT));
  lv_obj_set_pos(wifiStatus, 12, 40);
  lv_obj_set_size(wifiStatus, 270, 190);
  lv_obj_t *disconnectButton = makeButton(statusPanel, "TRENNEN", 12, 244,
                                          128, 42, redDark(), &ui_font_de_14);
  lv_obj_add_event_cb(disconnectButton, wifiDisconnectEvent, LV_EVENT_CLICKED,
                      nullptr);
  lv_obj_t *apButton = makeButton(statusPanel, "ACCESS POINT", 150, 244,
                                  128, 42, darkGrey(), &ui_font_de_14);
  lv_obj_add_event_cb(apButton, wifiApEvent, LV_EVENT_CLICKED, nullptr);

  lv_obj_t *configPanel = lv_obj_create(wifiPage);
  lv_obj_set_pos(configPanel, 324, 52);
  lv_obj_set_size(configPanel, 428, 300);
  stylePanel(configPanel, 10);
  makeLabel(configPanel, "WLAN-Konfiguration", &ui_font_de_18,
            lv_color_white());

  makeLabel(configPanel, "Passwort", &ui_font_de_14, lightGrey());
  wifiPassword = lv_textarea_create(configPanel);
  lv_obj_set_pos(wifiPassword, 10, 48);
  lv_obj_set_size(wifiPassword, 318, 44);
  lv_textarea_set_one_line(wifiPassword, true);
  lv_textarea_set_password_mode(wifiPassword, true);
  lv_textarea_set_placeholder_text(wifiPassword, "WLAN-Passwort");
  lv_obj_set_style_text_font(wifiPassword, &ui_font_de_14, 0);
  lv_obj_add_event_cb(wifiPassword, passwordFocusEvent, LV_EVENT_CLICKED,
                      nullptr);
  lv_obj_add_event_cb(wifiPassword, passwordChangedEvent, LV_EVENT_VALUE_CHANGED,
                      nullptr);

  wifiEyeButton = makeButton(configPanel, LV_SYMBOL_EYE_OPEN, 338, 48, 64, 44,
                             darkGrey(), &lv_font_montserrat_18);
  lv_obj_add_event_cb(wifiEyeButton, passwordEyeEvent, LV_EVENT_CLICKED, nullptr);
  wifiPasswordCount = makeLabel(configPanel, "0 Zeichen", &ui_font_de_14,
                                lightGrey());
  lv_obj_set_pos(wifiPasswordCount, 12, 96);

  makeLabel(configPanel, "Netzwerk", &ui_font_de_14, lightGrey());
  lv_obj_t *networkLabel = lv_obj_get_child(configPanel,
      static_cast<int32_t>(lv_obj_get_child_count(configPanel) - 1));
  lv_obj_set_pos(networkLabel, 10, 120);

  wifiDropdown = lv_dropdown_create(configPanel);
  lv_obj_set_pos(wifiDropdown, 10, 145);
  lv_obj_set_size(wifiDropdown, 300, 42);
  lv_dropdown_set_options(wifiDropdown, "Netzwerke suchen");
  lv_obj_set_style_text_font(wifiDropdown, &ui_font_de_14, 0);
  wifiScanButton = makeButton(configPanel, LV_SYMBOL_REFRESH, 322, 145, 80, 42,
                              blue(), &lv_font_montserrat_18);
  lv_obj_add_event_cb(wifiScanButton, wifiScanEvent, LV_EVENT_CLICKED, nullptr);

  lv_obj_t *connectButton = makeButton(configPanel, "SPEICHERN & VERBINDEN",
                                       10, 218, 392, 48, blue(),
                                       &ui_font_de_14);
  lv_obj_add_event_cb(connectButton, wifiConnectEvent, LV_EVENT_CLICKED, nullptr);

  wifiKeyboard = lv_keyboard_create(box);
  lv_obj_set_size(wifiKeyboard, 760, 205);
  lv_obj_align(wifiKeyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_keyboard_set_map(wifiKeyboard, LV_KEYBOARD_MODE_TEXT_LOWER,
                      germanLowerMap, nullptr);
  lv_keyboard_set_map(wifiKeyboard, LV_KEYBOARD_MODE_TEXT_UPPER,
                      germanUpperMap, nullptr);
  lv_keyboard_set_mode(wifiKeyboard, LV_KEYBOARD_MODE_TEXT_LOWER);
  lv_obj_set_style_text_font(wifiKeyboard, &ui_font_de_14, LV_PART_ITEMS);
  lv_obj_add_event_cb(wifiKeyboard, keyboardEvent, LV_EVENT_READY, nullptr);
  lv_obj_add_event_cb(wifiKeyboard, keyboardEvent, LV_EVENT_CANCEL, nullptr);
  lv_obj_add_flag(wifiKeyboard, LV_OBJ_FLAG_HIDDEN);

  mqttPage = lv_obj_create(box);
  lv_obj_remove_style_all(mqttPage);
  lv_obj_set_size(mqttPage, 760, 402);
  lv_obj_set_pos(mqttPage, 0, 0);
  backButton(mqttPage);
  makeIconTitle(mqttPage, LV_SYMBOL_UPLOAD, "MQTT / HOME ASSISTANT", 6,
                lv_color_hex(0xA060FF));
  lv_obj_t *mqttPanel = lv_obj_create(mqttPage);
  lv_obj_set_pos(mqttPanel, 90, 80);
  lv_obj_set_size(mqttPanel, 580, 230);
  stylePanel(mqttPanel, 12);
  lv_obj_t *mqttText = makeLabel(
      mqttPanel,
      "MQTT-Funktion wird in Schritt 3 aktiviert.\n\n"
      "Geplant:\n- Broker, Port und Zugangsdaten\n- Topic-Präfix und Client-ID\n"
      "- Home-Assistant-Discovery\n- Availability / Last Will\n"
      "- optionale, geschützte Fernsteuerung\n\n"
      "Fernsteuerung bleibt standardmäßig deaktiviert.",
      &ui_font_de_16, lv_color_hex(TEXT_BRIGHT));
  lv_obj_set_pos(mqttText, 25, 22);

  lv_obj_add_flag(debugPage, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(wifiPage, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(mqttPage, LV_OBJ_FLAG_HIDDEN);
}

void updateSettings(uint32_t now) {
  char uptime[32];
  formatClock(now, uptime, sizeof(uptime));
  const uint32_t freeHeap = ESP.getFreeHeap();
  const uint32_t minHeap = ESP.getMinFreeHeap();
  const uint32_t freePsram = ESP.getFreePsram();
  const uint32_t totalPsram = ESP.getPsramSize();

  if (overviewMetrics) {
    char text[300];
    snprintf(text, sizeof(text),
             "RAM frei  %u kB     PSRAM frei  %.2f MB     CPU  %u %%     "
             "FPS  %u     Uptime  %s",
             freeHeap / 1024U, freePsram / 1048576.0f, cpuLoadPercent,
             Display::fps(), uptime);
    lv_label_set_text(overviewMetrics, text);
  }

  if (debugMetrics) {
    char text[520];
    snprintf(text, sizeof(text),
             "Freier RAM:        %u kB\nMinimum RAM:       %u kB\n"
             "Freier PSRAM:      %.2f MB\nPSRAM gesamt:      %.2f MB\n"
             "CPU-Last:          %u %%\nLVGL Flush-FPS:    %u fps\n"
             "Loop-Zeit:         %.2f ms\nUptime:            %s\n"
             "Reset-Grund:       %s",
             freeHeap / 1024U, minHeap / 1024U,
             freePsram / 1048576.0f, totalPsram / 1048576.0f,
             cpuLoadPercent, Display::fps(), loopTimeUs / 1000.0f,
             uptime, resetReasonText());
    lv_label_set_text(debugMetrics, text);
  }

  if (settingsPage == SettingsPage::Wifi && wifiStatus) {
    if (wifiOptionsDirty && HmiWifi::scanReady()) rebuildWifiOptions();
    char text[520];
    snprintf(text, sizeof(text),
             "WLAN: %s\nSSID: %s\nSignal: %ld dBm (%u %%)\nIP: %s\n"
             "Gateway: %s\nMAC: %s\nNTP: %s\nZeit: %s\nAP: %s",
             HmiWifi::statusText().c_str(), HmiWifi::ssid().c_str(),
             static_cast<long>(HmiWifi::rssi()), HmiWifi::signalPercent(),
             HmiWifi::ipText().c_str(), HmiWifi::gatewayText().c_str(),
             HmiWifi::macText().c_str(),
             HmiWifi::timeValid() ? "synchronisiert" : "nicht verfügbar",
             HmiWifi::localTimeText().c_str(),
             HmiWifi::accessPointActive() ? "aktiv" : "aus");
    lv_label_set_text(wifiStatus, text);
  }
}

void updateTouchSettings(const TouchSample &) {}
}  // namespace UiInternal
