#include "ui_internal.h"
#include "app_config.h"
#include "display.h"
#include "hmi_wifi.h"
#include <ESP.h>
#include <esp_heap_caps.h>
#include <cstring>

namespace UiInternal {
namespace {
lv_obj_t *overview=nullptr,*debugPage=nullptr,*wifiPage=nullptr,*mqttPage=nullptr;
lv_obj_t *metrics=nullptr,*debugMetrics=nullptr,*wifiStatus=nullptr,*wifiScanState=nullptr;
lv_obj_t *wifiDropdown=nullptr,*wifiPassword=nullptr,*wifiCount=nullptr,*wifiEye=nullptr,*wifiKeyboard=nullptr;
lv_obj_t *wifiNetworkLabel=nullptr,*wifiScanButton=nullptr,*wifiSelectedLabel=nullptr;
bool wifiOptionsDirty=true;
bool passwordVisible=false;
bool keyboardMode=false;
enum class Page:uint8_t{Overview,Debug,Wifi,Mqtt};

void logMemory(const char *tag){
  DebugLog::printf("[WIFI-UI] %s heap=%u intFree=%u intLargest=%u psFree=%u psLargest=%u\n",
    tag,ESP.getFreeHeap(),
    (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
    (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
    (unsigned)heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
    (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM));
}

String selectedNetwork(){
  if(!wifiDropdown)return String();
  char selected[96]={0};
  lv_dropdown_get_selected_str(wifiDropdown,selected,sizeof(selected));
  String ssid=selected;
  const int marker=ssid.lastIndexOf("  (");
  if(marker>0)ssid=ssid.substring(0,marker);
  if(ssid=="Netzwerke suchen"||ssid=="Keine Netzwerke gefunden")ssid="";
  return ssid;
}

void setKeyboardMode(bool active){
  keyboardMode=active;
  lv_obj_t *normal[]={wifiNetworkLabel,wifiDropdown,wifiScanButton,wifiScanState};
  for(auto *obj:normal)if(obj){if(active)lv_obj_add_flag(obj,LV_OBJ_FLAG_HIDDEN);else lv_obj_clear_flag(obj,LV_OBJ_FLAG_HIDDEN);}
  if(wifiSelectedLabel){
    if(active){
      String ssid=selectedNetwork();
      if(ssid.isEmpty())ssid="Noch kein Netzwerk ausgewählt";
      String text="Ausgewähltes Netzwerk:  "+ssid;
      lv_label_set_text(wifiSelectedLabel,text.c_str());
      lv_obj_clear_flag(wifiSelectedLabel,LV_OBJ_FLAG_HIDDEN);
    }else lv_obj_add_flag(wifiSelectedLabel,LV_OBJ_FLAG_HIDDEN);
  }
}

void hideKeyboard(){
  if(!wifiKeyboard)return;
  lv_keyboard_set_textarea(wifiKeyboard,nullptr);
  lv_obj_add_flag(wifiKeyboard,LV_OBJ_FLAG_HIDDEN);
  setKeyboardMode(false);
  logMemory("Standardtastatur ausgeblendet");
}

void show(Page page){
  if(page!=Page::Wifi)hideKeyboard();
  lv_obj_t *all[]={overview,debugPage,wifiPage,mqttPage};
  for(auto *o:all)if(o)lv_obj_add_flag(o,LV_OBJ_FLAG_HIDDEN);
  lv_obj_t *target=overview;
  if(page==Page::Debug)target=debugPage;
  else if(page==Page::Wifi)target=wifiPage;
  else if(page==Page::Mqtt)target=mqttPage;
  if(target)lv_obj_clear_flag(target,LV_OBJ_FLAG_HIDDEN);
}
void nav(lv_event_t *e){show(static_cast<Page>(reinterpret_cast<intptr_t>(lv_event_get_user_data(e))));}

lv_obj_t *back(lv_obj_t *p){
  lv_obj_t *b=makeButton(p,"ZURÜCK",8,5,125,36,darkGrey(),&ui_font_de_14);
  lv_obj_add_event_cb(b,nav,LV_EVENT_CLICKED,reinterpret_cast<void*>(static_cast<intptr_t>(Page::Overview)));
  return b;
}

lv_obj_t *card(lv_obj_t *p,const char *icon,const char *title,const char *text,int x,lv_color_t color,Page page){
  lv_obj_t *b=lv_button_create(p);lv_obj_remove_style_all(b);lv_obj_set_pos(b,x,58);lv_obj_set_size(b,230,218);
  lv_obj_set_style_bg_color(b,lv_color_hex(0x0D1823),0);lv_obj_set_style_bg_opa(b,LV_OPA_COVER,0);
  lv_obj_set_style_border_color(b,border(),0);lv_obj_set_style_border_width(b,2,0);lv_obj_set_style_radius(b,12,0);
  lv_obj_t *i=makeLabel(b,icon,&lv_font_montserrat_24,color);lv_obj_align(i,LV_ALIGN_TOP_MID,0,18);
  lv_obj_t *h=makeLabel(b,title,&ui_font_de_18,lv_color_white());lv_obj_align(h,LV_ALIGN_TOP_MID,0,60);
  lv_obj_t *d=makeLabel(b,text,&ui_font_de_14,lv_color_hex(0xE8EEF4));lv_obj_set_width(d,195);lv_label_set_long_mode(d,LV_LABEL_LONG_WRAP);lv_obj_set_style_text_align(d,LV_TEXT_ALIGN_CENTER,0);lv_obj_align(d,LV_ALIGN_TOP_MID,0,102);
  lv_obj_add_event_cb(b,nav,LV_EVENT_CLICKED,reinterpret_cast<void*>(static_cast<intptr_t>(page)));
  return b;
}

void updateCount(){if(!wifiCount||!wifiPassword)return;char text[24];snprintf(text,sizeof(text),"%u Zeichen",(unsigned)std::strlen(lv_textarea_get_text(wifiPassword)));lv_label_set_text(wifiCount,text);}
void keyboardEvent(lv_event_t *e){const lv_event_code_t code=lv_event_get_code(e);if(code==LV_EVENT_READY||code==LV_EVENT_CANCEL)hideKeyboard();}

void passwordFocus(lv_event_t *){
  if(!wifiKeyboard){DebugLog::println("[WIFI-UI] Standardtastatur nicht verfügbar");return;}
  setKeyboardMode(true);
  logMemory("Standardtastatur vor Einblenden");
  lv_keyboard_set_textarea(wifiKeyboard,wifiPassword);
  lv_keyboard_set_mode(wifiKeyboard,LV_KEYBOARD_MODE_TEXT_LOWER);
  lv_obj_clear_flag(wifiKeyboard,LV_OBJ_FLAG_HIDDEN);
  lv_obj_move_foreground(wifiKeyboard);
  lv_obj_invalidate(wifiKeyboard);
  DebugLog::printf("[WIFI-UI] Tastatur eingeblendet, Netzwerk='%s'\n",selectedNetwork().c_str());
  logMemory("Standardtastatur eingeblendet");
}

void passwordChanged(lv_event_t *){updateCount();}
void passwordEye(lv_event_t *){passwordVisible=!passwordVisible;lv_textarea_set_password_mode(wifiPassword,!passwordVisible);lv_obj_set_style_bg_color(wifiEye,passwordVisible?greenDark():darkGrey(),0);}
void scanEvent(lv_event_t *){hideKeyboard();HmiWifi::requestScan();wifiOptionsDirty=true;if(wifiScanState)lv_label_set_text(wifiScanState,"Suche läuft ...");DebugLog::println("[WIFI-UI] Netzwerksuche angefordert");}
void disconnectEvent(lv_event_t *){HmiWifi::disconnect();}
void connectEvent(lv_event_t *){
  const String ssid=selectedNetwork();
  if(ssid.isEmpty()){DebugLog::println("[WIFI-UI] Kein gültiges WLAN ausgewählt");return;}
  HmiWifi::connectTo(ssid,String(lv_textarea_get_text(wifiPassword)));hideKeyboard();
}

void rebuildWifiOptions(){
  if(!wifiDropdown||!HmiWifi::scanReady())return;
  String options;const int count=HmiWifi::scanCount();
  for(int i=0;i<count&&i<20;++i){const String name=HmiWifi::scanSsid(i);if(name.isEmpty())continue;if(options.length())options+='\n';options+=name;options+="  (";options+=String(HmiWifi::scanRssi(i));options+=" dBm";if(HmiWifi::scanEncrypted(i))options+=", gesichert";options+=")";}
  if(options.isEmpty())options="Keine Netzwerke gefunden";
  lv_dropdown_set_options(wifiDropdown,options.c_str());wifiOptionsDirty=false;
  if(wifiScanState){char text[40];snprintf(text,sizeof(text),"%d Netzwerk(e) gefunden",count);lv_label_set_text(wifiScanState,text);}
  DebugLog::printf("[WIFI-UI] %d Netzwerke in Liste übernommen\n",count);
}
}

void makeSettings(lv_obj_t *parent){
  lv_obj_t *box=lv_obj_create(parent);lv_obj_set_size(box,780,422);stylePanel(box);
  overview=lv_obj_create(box);lv_obj_remove_style_all(overview);lv_obj_set_size(overview,760,402);
  lv_obj_t *title=makeLabel(overview,"EINSTELLUNGEN",&ui_font_de_24,lv_color_white());lv_obj_align(title,LV_ALIGN_TOP_MID,0,6);
  card(overview,LV_SYMBOL_EDIT,"Debug-Einstellungen","Systemdiagnose und Speicherwerte anzeigen.",8,blue(),Page::Debug);
  card(overview,LV_SYMBOL_WIFI,"WiFi","WLAN suchen, auswählen und sicher verbinden.",265,green(),Page::Wifi);
  card(overview,LV_SYMBOL_UPLOAD,"MQTT / Home Assistant","Vorbereitung für Stufe 3.",522,lv_color_hex(0xA060FF),Page::Mqtt);
  metrics=makeLabel(overview,"",&ui_font_de_14,lv_color_hex(0xE8EEF4));lv_obj_set_pos(metrics,15,302);lv_obj_set_size(metrics,730,70);

  debugPage=lv_obj_create(box);lv_obj_remove_style_all(debugPage);lv_obj_set_size(debugPage,760,402);back(debugPage);
  lv_obj_t *dh=makeLabel(debugPage,"DEBUG-EINSTELLUNGEN",&ui_font_de_24,blue());lv_obj_align(dh,LV_ALIGN_TOP_MID,0,8);
  lv_obj_t *dp=lv_obj_create(debugPage);lv_obj_set_pos(dp,40,65);lv_obj_set_size(dp,680,285);stylePanel(dp,10);
  debugMetrics=makeLabel(dp,"",&ui_font_de_16,lv_color_hex(0xE8EEF4));lv_obj_set_pos(debugMetrics,15,15);

  wifiPage=lv_obj_create(box);lv_obj_remove_style_all(wifiPage);lv_obj_set_size(wifiPage,760,402);back(wifiPage);
  lv_obj_t *wh=makeLabel(wifiPage,"WIFI",&ui_font_de_24,green());lv_obj_set_pos(wh,145,7);

  lv_obj_t *statusPanel=lv_obj_create(wifiPage);lv_obj_set_pos(statusPanel,8,42);lv_obj_set_size(statusPanel,210,155);stylePanel(statusPanel,10);
  wifiStatus=makeLabel(statusPanel,"",&ui_font_de_14,lv_color_hex(0xE8EEF4));lv_obj_set_pos(wifiStatus,7,7);lv_obj_set_width(wifiStatus,188);
  lv_obj_t *disconnect=makeButton(statusPanel,"TRENNEN",7,111,90,34,redDark(),&ui_font_de_14);lv_obj_add_event_cb(disconnect,disconnectEvent,LV_EVENT_CLICKED,nullptr);

  lv_obj_t *config=lv_obj_create(wifiPage);lv_obj_set_pos(config,226,42);lv_obj_set_size(config,526,155);stylePanel(config,10);
  makeLabel(config,"WLAN-Konfiguration",&ui_font_de_18,lv_color_white());
  wifiNetworkLabel=makeLabel(config,"Verfügbare Netzwerke",&ui_font_de_14,lightGrey());lv_obj_set_pos(wifiNetworkLabel,10,28);
  wifiScanState=makeLabel(config,"Noch keine Suche",&ui_font_de_14,lightGrey());lv_obj_set_pos(wifiScanState,305,28);
  wifiDropdown=lv_dropdown_create(config);lv_obj_set_pos(wifiDropdown,10,48);lv_obj_set_size(wifiDropdown,402,38);lv_dropdown_set_options(wifiDropdown,"Netzwerke suchen");lv_obj_set_style_text_font(wifiDropdown,&ui_font_de_14,0);
  wifiScanButton=makeButton(config,LV_SYMBOL_REFRESH,422,48,78,38,blue(),&lv_font_montserrat_18);lv_obj_add_event_cb(wifiScanButton,scanEvent,LV_EVENT_CLICKED,nullptr);
  wifiSelectedLabel=makeLabel(config,"",&ui_font_de_16,lv_color_white());lv_obj_set_pos(wifiSelectedLabel,10,31);lv_obj_set_width(wifiSelectedLabel,490);lv_obj_add_flag(wifiSelectedLabel,LV_OBJ_FLAG_HIDDEN);

  lv_obj_t *passwordLabel=makeLabel(config,"Passwort",&ui_font_de_14,lightGrey());lv_obj_set_pos(passwordLabel,10,90);
  wifiCount=makeLabel(config,"0 Zeichen",&ui_font_de_14,lightGrey());lv_obj_set_pos(wifiCount,112,90);
  wifiPassword=lv_textarea_create(config);lv_obj_set_pos(wifiPassword,10,108);lv_obj_set_size(wifiPassword,300,38);lv_textarea_set_one_line(wifiPassword,true);lv_textarea_set_password_mode(wifiPassword,true);lv_textarea_set_placeholder_text(wifiPassword,"WLAN-Passwort");lv_obj_set_style_text_font(wifiPassword,&ui_font_de_14,0);
  lv_obj_add_event_cb(wifiPassword,passwordFocus,LV_EVENT_CLICKED,nullptr);lv_obj_add_event_cb(wifiPassword,passwordFocus,LV_EVENT_FOCUSED,nullptr);lv_obj_add_event_cb(wifiPassword,passwordChanged,LV_EVENT_VALUE_CHANGED,nullptr);
  wifiEye=makeButton(config,LV_SYMBOL_EYE_OPEN,318,108,50,38,darkGrey(),&lv_font_montserrat_18);lv_obj_add_event_cb(wifiEye,passwordEye,LV_EVENT_CLICKED,nullptr);
  lv_obj_t *connect=makeButton(config,"VERBINDEN",378,108,122,38,blue(),&ui_font_de_14);lv_obj_add_event_cb(connect,connectEvent,LV_EVENT_CLICKED,nullptr);

  logMemory("Standardtastatur vor einmaligem Erzeugen");
  wifiKeyboard=lv_keyboard_create(box);
  if(wifiKeyboard){lv_obj_set_size(wifiKeyboard,760,205);lv_obj_align(wifiKeyboard,LV_ALIGN_BOTTOM_MID,0,0);lv_keyboard_set_mode(wifiKeyboard,LV_KEYBOARD_MODE_TEXT_LOWER);lv_keyboard_set_textarea(wifiKeyboard,nullptr);lv_obj_add_event_cb(wifiKeyboard,keyboardEvent,LV_EVENT_READY,nullptr);lv_obj_add_event_cb(wifiKeyboard,keyboardEvent,LV_EVENT_CANCEL,nullptr);lv_obj_add_flag(wifiKeyboard,LV_OBJ_FLAG_HIDDEN);logMemory("Standardtastatur einmalig erzeugt und versteckt");}
  else DebugLog::println("[WIFI-UI] Standardtastatur konnte beim Boot nicht erzeugt werden");

  mqttPage=lv_obj_create(box);lv_obj_remove_style_all(mqttPage);lv_obj_set_size(mqttPage,760,402);back(mqttPage);
  lv_obj_t *mh=makeLabel(mqttPage,"MQTT / HOME ASSISTANT",&ui_font_de_24,lv_color_hex(0xA060FF));lv_obj_align(mh,LV_ALIGN_TOP_MID,0,8);
  lv_obj_t *mt=makeLabel(mqttPage,"Diese Funktion folgt in Stufe 3.",&ui_font_de_18,lv_color_white());lv_obj_align(mt,LV_ALIGN_CENTER,0,0);
  lv_obj_add_flag(debugPage,LV_OBJ_FLAG_HIDDEN);lv_obj_add_flag(wifiPage,LV_OBJ_FLAG_HIDDEN);lv_obj_add_flag(mqttPage,LV_OBJ_FLAG_HIDDEN);
}

void updateSettings(uint32_t now){
  char uptime[32];formatClock(now,uptime,sizeof(uptime));
  const size_t intTotal=heap_caps_get_total_size(MALLOC_CAP_INTERNAL),intFree=heap_caps_get_free_size(MALLOC_CAP_INTERNAL),intLargest=heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL);
  const size_t psTotal=heap_caps_get_total_size(MALLOC_CAP_SPIRAM),psFree=heap_caps_get_free_size(MALLOC_CAP_SPIRAM),psLargest=heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM);
  if(metrics){char t[220];snprintf(t,sizeof(t),"RAM frei %u kB   PSRAM frei %u kB   CPU %u %%   FPS %u   Uptime %s",(unsigned)(intFree/1024),(unsigned)(psFree/1024),cpuLoadPercent,Display::fps(),uptime);lv_label_set_text(metrics,t);}
  if(debugMetrics){char t[520];snprintf(t,sizeof(t),"Interner RAM gesamt: %u kB\nInterner RAM frei: %u kB\nGrößter interner Block: %u kB\n\nPSRAM gesamt: %u kB\nPSRAM frei: %u kB\nGrößter PSRAM-Block: %u kB\n\nArduino Heap frei: %u Byte\nCPU-Last: %u %%\nFPS: %u",(unsigned)(intTotal/1024),(unsigned)(intFree/1024),(unsigned)(intLargest/1024),(unsigned)(psTotal/1024),(unsigned)(psFree/1024),(unsigned)(psLargest/1024),ESP.getFreeHeap(),cpuLoadPercent,Display::fps());lv_label_set_text(debugMetrics,t);}
  if(wifiOptionsDirty&&HmiWifi::scanReady())rebuildWifiOptions();
  if(wifiScanState&&HmiWifi::scanRunning())lv_label_set_text(wifiScanState,"Suche läuft ...");
  if(wifiStatus){char t[300];snprintf(t,sizeof(t),"Status: %s\nSSID: %s\nSignal: %ld dBm\nIP: %s\nNTP: %s",HmiWifi::statusText().c_str(),HmiWifi::ssid().c_str(),(long)HmiWifi::rssi(),HmiWifi::ipText().c_str(),HmiWifi::timeValid()?"synchronisiert":"nicht verfügbar");lv_label_set_text(wifiStatus,t);}
}
void updateTouchSettings(const TouchSample &s){(void)s;}
}