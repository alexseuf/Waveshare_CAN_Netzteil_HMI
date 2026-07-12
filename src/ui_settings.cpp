#include "ui_internal.h"
#include "app_config.h"
#include "display.h"
#include "hmi_wifi.h"
#include <ESP.h>
#include <esp_heap_caps.h>

namespace UiInternal {
namespace {
lv_obj_t *overview=nullptr,*debugPage=nullptr,*wifiPage=nullptr,*mqttPage=nullptr;
lv_obj_t *metrics=nullptr,*debugMetrics=nullptr,*wifiStatus=nullptr;
enum class Page:uint8_t{Overview,Debug,Wifi,Mqtt};

void show(Page page){
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
}

void makeSettings(lv_obj_t *parent){
  lv_obj_t *box=lv_obj_create(parent);lv_obj_set_size(box,780,422);stylePanel(box);
  overview=lv_obj_create(box);lv_obj_remove_style_all(overview);lv_obj_set_size(overview,760,402);
  lv_obj_t *title=makeLabel(overview,"EINSTELLUNGEN",&ui_font_de_24,lv_color_white());lv_obj_align(title,LV_ALIGN_TOP_MID,0,6);
  card(overview,LV_SYMBOL_EDIT,"Debug-Einstellungen","Systemdiagnose und Speicherwerte anzeigen.",8,blue(),Page::Debug);
  card(overview,LV_SYMBOL_WIFI,"WiFi","WLAN-Status anzeigen. Eingabe folgt dynamisch in der nächsten Stufe.",265,green(),Page::Wifi);
  card(overview,LV_SYMBOL_UPLOAD,"MQTT / Home Assistant","Vorbereitung für Stufe 3.",522,lv_color_hex(0xA060FF),Page::Mqtt);
  metrics=makeLabel(overview,"",&ui_font_de_14,lv_color_hex(0xE8EEF4));lv_obj_set_pos(metrics,15,302);lv_obj_set_size(metrics,730,70);

  debugPage=lv_obj_create(box);lv_obj_remove_style_all(debugPage);lv_obj_set_size(debugPage,760,402);back(debugPage);
  lv_obj_t *dh=makeLabel(debugPage,"DEBUG-EINSTELLUNGEN",&ui_font_de_24,blue());lv_obj_align(dh,LV_ALIGN_TOP_MID,0,8);
  lv_obj_t *dp=lv_obj_create(debugPage);lv_obj_set_pos(dp,40,65);lv_obj_set_size(dp,680,285);stylePanel(dp,10);
  debugMetrics=makeLabel(dp,"",&ui_font_de_16,lv_color_hex(0xE8EEF4));lv_obj_set_pos(debugMetrics,15,15);

  wifiPage=lv_obj_create(box);lv_obj_remove_style_all(wifiPage);lv_obj_set_size(wifiPage,760,402);back(wifiPage);
  lv_obj_t *wh=makeLabel(wifiPage,"WIFI",&ui_font_de_24,green());lv_obj_align(wh,LV_ALIGN_TOP_MID,0,8);
  lv_obj_t *wp=lv_obj_create(wifiPage);lv_obj_set_pos(wp,75,65);lv_obj_set_size(wp,610,285);stylePanel(wp,10);
  wifiStatus=makeLabel(wp,"",&ui_font_de_16,lv_color_hex(0xE8EEF4));lv_obj_set_pos(wifiStatus,18,18);

  mqttPage=lv_obj_create(box);lv_obj_remove_style_all(mqttPage);lv_obj_set_size(mqttPage,760,402);back(mqttPage);
  lv_obj_t *mh=makeLabel(mqttPage,"MQTT / HOME ASSISTANT",&ui_font_de_24,lv_color_hex(0xA060FF));lv_obj_align(mh,LV_ALIGN_TOP_MID,0,8);
  lv_obj_t *mt=makeLabel(mqttPage,"Diese Funktion folgt in Stufe 3.",&ui_font_de_18,lv_color_white());lv_obj_align(mt,LV_ALIGN_CENTER,0,0);

  lv_obj_add_flag(debugPage,LV_OBJ_FLAG_HIDDEN);lv_obj_add_flag(wifiPage,LV_OBJ_FLAG_HIDDEN);lv_obj_add_flag(mqttPage,LV_OBJ_FLAG_HIDDEN);
}

void updateSettings(uint32_t now){
  char uptime[32];formatClock(now,uptime,sizeof(uptime));
  const size_t intTotal=heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
  const size_t intFree=heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
  const size_t intLargest=heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL);
  const size_t psTotal=heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
  const size_t psFree=heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
  const size_t psLargest=heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM);
  if(metrics){char t[220];snprintf(t,sizeof(t),"RAM frei %u kB   PSRAM frei %u kB   CPU %u %%   FPS %u   Uptime %s",(unsigned)(intFree/1024),(unsigned)(psFree/1024),cpuLoadPercent,Display::fps(),uptime);lv_label_set_text(metrics,t);}
  if(debugMetrics){char t[520];snprintf(t,sizeof(t),"Interner RAM gesamt: %u kB\nInterner RAM frei: %u kB\nGrößter interner Block: %u kB\n\nPSRAM gesamt: %u kB\nPSRAM frei: %u kB\nGrößter PSRAM-Block: %u kB\n\nArduino Heap frei: %u Byte\nCPU-Last: %u %%\nFPS: %u",(unsigned)(intTotal/1024),(unsigned)(intFree/1024),(unsigned)(intLargest/1024),(unsigned)(psTotal/1024),(unsigned)(psFree/1024),(unsigned)(psLargest/1024),ESP.getFreeHeap(),cpuLoadPercent,Display::fps());lv_label_set_text(debugMetrics,t);}
  if(wifiStatus){char t[420];snprintf(t,sizeof(t),"Status: %s\nSSID: %s\nSignal: %ld dBm\nIP: %s\nGateway: %s\nMAC: %s\nNTP: %s\nZeit: %s\n\nPassworteingabe und Tastatur sind in dieser Testversion noch deaktiviert.",HmiWifi::statusText().c_str(),HmiWifi::ssid().c_str(),(long)HmiWifi::rssi(),HmiWifi::ipText().c_str(),HmiWifi::gatewayText().c_str(),HmiWifi::macText().c_str(),HmiWifi::timeValid()?"synchronisiert":"nicht verfügbar",HmiWifi::localTimeText().c_str());lv_label_set_text(wifiStatus,t);}
}
void updateTouchSettings(const TouchSample &s){(void)s;}
}
