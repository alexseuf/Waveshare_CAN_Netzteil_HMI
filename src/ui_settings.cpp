#include "ui_internal.h"
#include "app_config.h"
#include "can_driver.h"
#include "display.h"
#include <ESP.h>
#include <esp_system.h>

namespace UiInternal {
static lv_obj_t *infoText=nullptr,*touchText=nullptr,*infoSystem=nullptr,*infoCan=nullptr;
static lv_obj_t *settingsOverview=nullptr,*debugPage=nullptr,*wifiPage=nullptr,*mqttPage=nullptr;
static lv_obj_t *overviewMetrics=nullptr,*debugMetrics=nullptr;
static bool showCanLogger=false;

static constexpr uint32_t TEXT_BRIGHT = 0xE8EEF4;
static constexpr uint32_t TEXT_SECONDARY = 0xD4DEE7;
static constexpr uint32_t CHECK_BG = 0x182431;
static constexpr uint32_t CHECK_BORDER = 0x8EA0B0;

enum class SettingsPage : uint8_t { Overview, Debug, Wifi, Mqtt };
static SettingsPage settingsPage=SettingsPage::Overview;

static void infoTab(lv_event_t *e){showCanLogger=static_cast<int>(reinterpret_cast<intptr_t>(lv_event_get_user_data(e)))==1;if(showCanLogger){lv_obj_add_flag(infoSystem,LV_OBJ_FLAG_HIDDEN);lv_obj_clear_flag(infoCan,LV_OBJ_FLAG_HIDDEN);}else{lv_obj_clear_flag(infoSystem,LV_OBJ_FLAG_HIDDEN);lv_obj_add_flag(infoCan,LV_OBJ_FLAG_HIDDEN);}}
static void logOption(lv_event_t *e){lv_obj_t *cb=(lv_obj_t*)lv_event_get_target(e);uint16_t cat=(uint16_t)(uintptr_t)lv_event_get_user_data(e);uint16_t mask=DebugLog::categoryMask();if(lv_obj_has_state(cb,LV_STATE_CHECKED))mask|=cat;else mask&=(uint16_t)~cat;DebugLog::setCategoryMask(mask);}
static void outOption(lv_event_t *e){lv_obj_t *cb=(lv_obj_t*)lv_event_get_target(e);int id=(int)(intptr_t)lv_event_get_user_data(e);bool on=lv_obj_has_state(cb,LV_STATE_CHECKED);if(id==1)DebugLog::setDisplayEnabled(on);else DebugLog::setUartEnabled(on);}

static void styleCheckbox(lv_obj_t *cb){
  lv_obj_set_style_text_font(cb,&ui_font_de_14,LV_PART_MAIN);
  lv_obj_set_style_text_color(cb,lv_color_hex(TEXT_BRIGHT),LV_PART_MAIN);
  lv_obj_set_style_text_opa(cb,LV_OPA_COVER,LV_PART_MAIN);
  lv_obj_set_style_bg_color(cb,lv_color_hex(CHECK_BG),LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(cb,LV_OPA_COVER,LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(cb,lv_color_hex(CHECK_BORDER),LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(cb,2,LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(cb,4,LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(cb,blue(),LV_PART_INDICATOR | LV_STATE_CHECKED);
  lv_obj_set_style_border_color(cb,lv_color_lighten(blue(),35),LV_PART_INDICATOR | LV_STATE_CHECKED);
  lv_obj_set_style_text_color(cb,lv_color_white(),LV_PART_MAIN | LV_STATE_CHECKED);
}

static lv_obj_t *opt(lv_obj_t *p,const char *t,int x,int y,uint16_t c){
  lv_obj_t *cb=lv_checkbox_create(p);lv_checkbox_set_text(cb,t);lv_obj_set_pos(cb,x,y);styleCheckbox(cb);
  if(DebugLog::categoryEnabled(c))lv_obj_add_state(cb,LV_STATE_CHECKED);
  lv_obj_add_event_cb(cb,logOption,LV_EVENT_VALUE_CHANGED,(void*)(uintptr_t)c);return cb;
}

static void showSettingsPage(SettingsPage page){
  settingsPage=page;
  lv_obj_t *all[]={settingsOverview,debugPage,wifiPage,mqttPage};
  for(auto *obj:all)if(obj)lv_obj_add_flag(obj,LV_OBJ_FLAG_HIDDEN);
  lv_obj_t *target=settingsOverview;
  if(page==SettingsPage::Debug)target=debugPage;
  else if(page==SettingsPage::Wifi)target=wifiPage;
  else if(page==SettingsPage::Mqtt)target=mqttPage;
  if(target)lv_obj_clear_flag(target,LV_OBJ_FLAG_HIDDEN);
}
static void settingsNav(lv_event_t *e){showSettingsPage(static_cast<SettingsPage>(reinterpret_cast<intptr_t>(lv_event_get_user_data(e))));}

static lv_obj_t *makeIconTitle(lv_obj_t *parent,const char *icon,const char *text,int y,lv_color_t color){
  lv_obj_t *group=lv_obj_create(parent);lv_obj_remove_style_all(group);lv_obj_set_size(group,420,38);lv_obj_set_pos(group,170,y);lv_obj_clear_flag(group,LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_t *i=makeLabel(group,icon,&lv_font_montserrat_24,color);lv_obj_set_pos(i,0,4);
  lv_obj_t *t=makeLabel(group,text,&ui_font_de_24,color);lv_obj_set_pos(t,42,0);
  return group;
}

static lv_obj_t *card(lv_obj_t *parent,const char *icon,const char *title,const char *description,int x,lv_color_t accent,SettingsPage target){
  lv_obj_t *b=lv_button_create(parent);lv_obj_remove_style_all(b);lv_obj_set_pos(b,x,54);lv_obj_set_size(b,230,222);lv_obj_add_flag(b,LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_bg_color(b,lv_color_hex(0x0D1823),0);lv_obj_set_style_bg_opa(b,LV_OPA_COVER,0);lv_obj_set_style_border_color(b,border(),0);lv_obj_set_style_border_width(b,2,0);lv_obj_set_style_radius(b,12,0);
  lv_obj_set_style_bg_color(b,lv_color_hex(0x14283A),LV_STATE_PRESSED);
  lv_obj_t *i=makeLabel(b,icon,&lv_font_montserrat_24,accent);lv_obj_align(i,LV_ALIGN_TOP_MID,0,18);
  lv_obj_t *h=makeLabel(b,title,&ui_font_de_18,lv_color_white());lv_obj_align(h,LV_ALIGN_TOP_MID,0,62);
  lv_obj_t *d=makeLabel(b,description,&ui_font_de_14,lv_color_hex(TEXT_BRIGHT));lv_label_set_long_mode(d,LV_LABEL_LONG_WRAP);lv_obj_set_width(d,198);lv_obj_set_style_text_align(d,LV_TEXT_ALIGN_CENTER,0);lv_obj_align(d,LV_ALIGN_TOP_MID,0,102);
  lv_obj_t *arrow=makeLabel(b,LV_SYMBOL_RIGHT,&lv_font_montserrat_18,lv_color_white());lv_obj_align(arrow,LV_ALIGN_BOTTOM_RIGHT,-14,-12);
  lv_obj_add_event_cb(b,settingsNav,LV_EVENT_CLICKED,reinterpret_cast<void*>(static_cast<intptr_t>(target)));
  return b;
}

static lv_obj_t *backButton(lv_obj_t *parent){
  lv_obj_t *b=lv_button_create(parent);lv_obj_remove_style_all(b);lv_obj_set_pos(b,8,5);lv_obj_set_size(b,132,36);lv_obj_add_flag(b,LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_bg_color(b,darkGrey(),LV_STATE_DEFAULT);lv_obj_set_style_bg_color(b,lv_color_darken(darkGrey(),25),LV_STATE_PRESSED);
  lv_obj_set_style_bg_opa(b,LV_OPA_COVER,0);lv_obj_set_style_border_color(b,border(),0);lv_obj_set_style_border_width(b,2,0);lv_obj_set_style_radius(b,9,0);
  lv_obj_t *icon=makeLabel(b,LV_SYMBOL_LEFT,&lv_font_montserrat_16,lv_color_white());lv_obj_set_pos(icon,14,9);
  lv_obj_t *text=makeLabel(b,"ZURÜCK",&ui_font_de_14,lv_color_white());lv_obj_set_pos(text,40,8);
  lv_obj_add_event_cb(b,settingsNav,LV_EVENT_CLICKED,reinterpret_cast<void*>(static_cast<intptr_t>(SettingsPage::Overview)));
  return b;
}

static const char *resetReasonText(){
  switch(esp_reset_reason()){
    case ESP_RST_POWERON:return "Power On";case ESP_RST_SW:return "Software";case ESP_RST_PANIC:return "Panic";
    case ESP_RST_INT_WDT:return "Interrupt WDT";case ESP_RST_TASK_WDT:return "Task WDT";case ESP_RST_WDT:return "Watchdog";
    case ESP_RST_DEEPSLEEP:return "Deep Sleep";case ESP_RST_BROWNOUT:return "Brownout";default:return "Unbekannt";
  }
}

void makeInfo(lv_obj_t *parent){
  lv_obj_t *box=lv_obj_create(parent);lv_obj_set_size(box,780,422);stylePanel(box);makeLabel(box,"INFO / DIAGNOSE",&lv_font_montserrat_24,blue());
  lv_obj_t *sys=makeButton(box,"SYSTEM",300,3,130,36,blue(),&lv_font_montserrat_16);lv_obj_add_event_cb(sys,infoTab,LV_EVENT_CLICKED,reinterpret_cast<void*>(0));
  lv_obj_t *can=makeButton(box,"CAN-LOGGER",440,3,150,36,blue(),&lv_font_montserrat_16);lv_obj_add_event_cb(can,infoTab,LV_EVENT_CLICKED,reinterpret_cast<void*>(1));
  infoSystem=lv_obj_create(box);lv_obj_remove_style_all(infoSystem);lv_obj_set_pos(infoSystem,6,50);lv_obj_set_size(infoSystem,748,344);infoText=makeLabel(infoSystem,"",&ui_font_de_16);lv_obj_set_pos(infoText,12,8);
  infoCan=lv_obj_create(box);lv_obj_remove_style_all(infoCan);lv_obj_set_pos(infoCan,6,50);lv_obj_set_size(infoCan,748,344);makeCan(infoCan);lv_obj_add_flag(infoCan,LV_OBJ_FLAG_HIDDEN);
}
bool infoCanVisible(){return showCanLogger;}
void updateInfo(const PowerSupplyState &s,uint32_t now){if(!infoText)return;char text[760];snprintf(text,sizeof(text),"Firmware: %s\nHeap frei: %u Byte\nPSRAM frei: %u Byte\nCAN-Treiber: %s / 250 kbit/s\nLink-State: %s\nTX-Zyklus: 1000 ms\nLetztes RX: %lu ms\nStatusbyte: 0x%02X\nEnergie: %.3f kWh\nTX-ID: 0x%08lX\nRX-ID: 0x%08lX",AppConfig::FIRMWARE_VERSION,ESP.getFreeHeap(),ESP.getFreePsram(),s.canStarted?"gestartet":"Fehler",s.linkStateText(),s.lastRxMs?now-s.lastRxMs:0,(unsigned)s.status,s.energyKWh,(unsigned long)AppConfig::CAN_ID_COMMAND,(unsigned long)AppConfig::CAN_ID_STATUS);lv_label_set_text(infoText,text);}

void makeSettings(lv_obj_t *parent){
  lv_obj_t *box=lv_obj_create(parent);lv_obj_set_size(box,780,422);stylePanel(box);

  settingsOverview=lv_obj_create(box);lv_obj_remove_style_all(settingsOverview);lv_obj_set_size(settingsOverview,760,402);lv_obj_set_pos(settingsOverview,0,0);lv_obj_clear_flag(settingsOverview,LV_OBJ_FLAG_SCROLLABLE);
  makeIconTitle(settingsOverview,LV_SYMBOL_SETTINGS,"EINSTELLUNGEN",5,lv_color_white());
  card(settingsOverview,LV_SYMBOL_EDIT,"Debug Settings","Logs, Diagnose, Anzeige und Entwickleroptionen konfigurieren.",8,blue(),SettingsPage::Debug);
  card(settingsOverview,LV_SYMBOL_WIFI,"WiFi","WLAN-Verbindung konfigurieren, Netzwerkstatus und NTP-Zeit anzeigen.",265,green(),SettingsPage::Wifi);
  card(settingsOverview,LV_SYMBOL_UPLOAD,"MQTT / Home Assistant","MQTT-Broker und Home-Assistant-Integration konfigurieren.",522,lv_color_hex(0xA060FF),SettingsPage::Mqtt);
  overviewMetrics=makeLabel(settingsOverview,"",&ui_font_de_14,lv_color_hex(TEXT_BRIGHT));lv_obj_set_pos(overviewMetrics,15,302);lv_obj_set_size(overviewMetrics,730,70);lv_obj_set_style_bg_color(overviewMetrics,lv_color_hex(0x0D1823),0);lv_obj_set_style_bg_opa(overviewMetrics,LV_OPA_COVER,0);lv_obj_set_style_border_color(overviewMetrics,border(),0);lv_obj_set_style_border_width(overviewMetrics,1,0);lv_obj_set_style_radius(overviewMetrics,10,0);lv_obj_set_style_pad_all(overviewMetrics,14,0);

  debugPage=lv_obj_create(box);lv_obj_remove_style_all(debugPage);lv_obj_set_size(debugPage,760,402);lv_obj_set_pos(debugPage,0,0);lv_obj_clear_flag(debugPage,LV_OBJ_FLAG_SCROLLABLE);backButton(debugPage);makeIconTitle(debugPage,LV_SYMBOL_EDIT,"DEBUG SETTINGS",6,blue());
  lv_obj_t *left=lv_obj_create(debugPage);lv_obj_set_pos(left,8,52);lv_obj_set_size(left,354,330);stylePanel(left,10);makeLabel(left,"Einstellungen",&ui_font_de_18,lv_color_white());
  opt(left,"System",12,42,DebugLog::SYSTEM);opt(left,"Fehler",12,72,DebugLog::ERROR);opt(left,"Warnungen",12,102,DebugLog::WARNING);opt(left,"CAN RX",170,42,DebugLog::CAN_RX);opt(left,"CAN TX",170,72,DebugLog::CAN_TX);opt(left,"Bedienung",170,102,DebugLog::UI);opt(left,"Touch",12,132,DebugLog::TOUCH);opt(left,"NVS / Speicher",170,132,DebugLog::STORAGE);
  lv_obj_t *d=lv_checkbox_create(left);lv_checkbox_set_text(d,"Ausgabe auf Display");lv_obj_set_pos(d,12,175);styleCheckbox(d);if(DebugLog::displayEnabled())lv_obj_add_state(d,LV_STATE_CHECKED);lv_obj_add_event_cb(d,outOption,LV_EVENT_VALUE_CHANGED,(void*)1);
  lv_obj_t *u=lv_checkbox_create(left);lv_checkbox_set_text(u,"Ausgabe über UART");lv_obj_set_pos(u,12,208);styleCheckbox(u);if(DebugLog::uartEnabled())lv_obj_add_state(u,LV_STATE_CHECKED);lv_obj_add_event_cb(u,outOption,LV_EVENT_VALUE_CHANGED,(void*)2);
  lv_obj_t *er=makeButton(left,"ENERGIE LÖSCHEN",12,254,150,45,blue(),&ui_font_de_14);lv_obj_add_event_cb(er,actionEvent,LV_EVENT_CLICKED,(void*)21);lv_obj_t *r=makeButton(left,"WERKSEINST.",176,254,150,45,redDark(),&ui_font_de_14);lv_obj_add_event_cb(r,actionEvent,LV_EVENT_CLICKED,(void*)20);
  lv_obj_t *right=lv_obj_create(debugPage);lv_obj_set_pos(right,378,52);lv_obj_set_size(right,374,330);stylePanel(right,10);makeLabel(right,"Systemdiagnose",&ui_font_de_18,lv_color_white());debugMetrics=makeLabel(right,"",&ui_font_de_16,lv_color_hex(TEXT_BRIGHT));lv_obj_set_pos(debugMetrics,12,42);lv_obj_set_size(debugMetrics,340,250);

  wifiPage=lv_obj_create(box);lv_obj_remove_style_all(wifiPage);lv_obj_set_size(wifiPage,760,402);lv_obj_set_pos(wifiPage,0,0);backButton(wifiPage);makeIconTitle(wifiPage,LV_SYMBOL_WIFI,"WIFI",6,green());lv_obj_t *wp=lv_obj_create(wifiPage);lv_obj_set_pos(wp,90,80);lv_obj_set_size(wp,580,230);stylePanel(wp,12);lv_obj_t *wt=makeLabel(wp,"WiFi-Funktion wird in Schritt 2 aktiviert.\n\nGeplant:\n- Netzwerksuche und Zugangsdaten\n- Status, Signalstärke und IP-Adresse\n- NTP-Zeitsynchronisation\n- Access-Point-Fallback\n\nCAN, Display und Ladebetrieb laufen unabhängig weiter.",&ui_font_de_16,lv_color_hex(TEXT_BRIGHT));lv_obj_set_pos(wt,25,22);

  mqttPage=lv_obj_create(box);lv_obj_remove_style_all(mqttPage);lv_obj_set_size(mqttPage,760,402);lv_obj_set_pos(mqttPage,0,0);backButton(mqttPage);makeIconTitle(mqttPage,LV_SYMBOL_UPLOAD,"MQTT / HOME ASSISTANT",6,lv_color_hex(0xA060FF));lv_obj_t *mp=lv_obj_create(mqttPage);lv_obj_set_pos(mp,90,80);lv_obj_set_size(mp,580,230);stylePanel(mp,12);lv_obj_t *mt=makeLabel(mp,"MQTT-Funktion wird in Schritt 3 aktiviert.\n\nGeplant:\n- Broker, Port und Zugangsdaten\n- Topic-Präfix und Client-ID\n- Home-Assistant-Discovery\n- Availability / Last Will\n- optionale, geschützte Fernsteuerung\n\nFernsteuerung bleibt standardmäßig deaktiviert.",&ui_font_de_16,lv_color_hex(TEXT_BRIGHT));lv_obj_set_pos(mt,25,22);

  lv_obj_add_flag(debugPage,LV_OBJ_FLAG_HIDDEN);lv_obj_add_flag(wifiPage,LV_OBJ_FLAG_HIDDEN);lv_obj_add_flag(mqttPage,LV_OBJ_FLAG_HIDDEN);
  touchText=nullptr;
}

void updateSettings(uint32_t now){
  char uptime[32];formatClock(now,uptime,sizeof(uptime));
  const uint32_t freeHeap=ESP.getFreeHeap();const uint32_t minHeap=ESP.getMinFreeHeap();const uint32_t freePsram=ESP.getFreePsram();const uint32_t totalPsram=ESP.getPsramSize();
  if(overviewMetrics){char text[300];snprintf(text,sizeof(text),"RAM frei  %u kB     PSRAM frei  %.2f MB     CPU  %u %%     FPS  %u     Uptime  %s",freeHeap/1024U,freePsram/1048576.0f,cpuLoadPercent,Display::fps(),uptime);lv_label_set_text(overviewMetrics,text);}
  if(debugMetrics){char text[520];snprintf(text,sizeof(text),"Freier RAM:        %u kB\nMinimum RAM:       %u kB\nFreier PSRAM:      %.2f MB\nPSRAM gesamt:      %.2f MB\nCPU-Last:          %u %%\nLVGL Flush-FPS:    %u fps\nLoop-Zeit:         %.2f ms\nUptime:            %s\nReset-Grund:       %s",freeHeap/1024U,minHeap/1024U,freePsram/1048576.0f,totalPsram/1048576.0f,cpuLoadPercent,Display::fps(),loopTimeUs/1000.0f,uptime,resetReasonText());lv_label_set_text(debugMetrics,text);}
}
void updateTouchSettings(const TouchSample &s){(void)s;}
}