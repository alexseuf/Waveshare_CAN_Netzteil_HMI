#include "ui_internal.h"
#include "app_config.h"
#include "can_driver.h"
#include <ESP.h>
namespace UiInternal {
static lv_obj_t *infoText=nullptr,*touchText=nullptr,*infoSystem=nullptr,*infoCan=nullptr;
static bool showCanLogger=false;
static void infoTab(lv_event_t *e){showCanLogger=static_cast<int>(reinterpret_cast<intptr_t>(lv_event_get_user_data(e)))==1;if(showCanLogger){lv_obj_add_flag(infoSystem,LV_OBJ_FLAG_HIDDEN);lv_obj_clear_flag(infoCan,LV_OBJ_FLAG_HIDDEN);}else{lv_obj_clear_flag(infoSystem,LV_OBJ_FLAG_HIDDEN);lv_obj_add_flag(infoCan,LV_OBJ_FLAG_HIDDEN);}}
static void logOption(lv_event_t *e){lv_obj_t *cb=(lv_obj_t*)lv_event_get_target(e);uint16_t cat=(uint16_t)(uintptr_t)lv_event_get_user_data(e);uint16_t mask=DebugLog::categoryMask();if(lv_obj_has_state(cb,LV_STATE_CHECKED))mask|=cat;else mask&=(uint16_t)~cat;DebugLog::setCategoryMask(mask);}
static void outOption(lv_event_t *e){lv_obj_t *cb=(lv_obj_t*)lv_event_get_target(e);int id=(int)(intptr_t)lv_event_get_user_data(e);bool on=lv_obj_has_state(cb,LV_STATE_CHECKED);if(id==1)DebugLog::setDisplayEnabled(on);else DebugLog::setUartEnabled(on);}
static lv_obj_t *opt(lv_obj_t *p,const char *t,int x,int y,uint16_t c){lv_obj_t *cb=lv_checkbox_create(p);lv_checkbox_set_text(cb,t);lv_obj_set_pos(cb,x,y);lv_obj_set_style_text_font(cb,&ui_font_de_16,0);if(DebugLog::categoryEnabled(c))lv_obj_add_state(cb,LV_STATE_CHECKED);lv_obj_add_event_cb(cb,logOption,LV_EVENT_VALUE_CHANGED,(void*)(uintptr_t)c);return cb;}
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
  lv_obj_t *box=lv_obj_create(parent);lv_obj_set_size(box,780,422);stylePanel(box);makeLabel(box,"EINSTELLUNGEN",&lv_font_montserrat_24,blue());lv_obj_t *h=makeLabel(box,"LOG-KATEGORIEN",&ui_font_de_18,green());lv_obj_set_pos(h,20,48);
  opt(box,"System",20,82,DebugLog::SYSTEM);opt(box,"Fehler",20,116,DebugLog::ERROR);opt(box,"Warnungen",20,150,DebugLog::WARNING);opt(box,"CAN RX",190,82,DebugLog::CAN_RX);opt(box,"CAN TX",190,116,DebugLog::CAN_TX);opt(box,"Bedienung",190,150,DebugLog::UI);opt(box,"Touch",360,82,DebugLog::TOUCH);opt(box,"NVS / Speicher",360,116,DebugLog::STORAGE);
  lv_obj_t *d=lv_checkbox_create(box);lv_checkbox_set_text(d,"Ausgabe auf Display");lv_obj_set_pos(d,20,200);lv_obj_set_style_text_font(d,&ui_font_de_16,0);if(DebugLog::displayEnabled())lv_obj_add_state(d,LV_STATE_CHECKED);lv_obj_add_event_cb(d,outOption,LV_EVENT_VALUE_CHANGED,(void*)1);
  lv_obj_t *u=lv_checkbox_create(box);lv_checkbox_set_text(u,"Ausgabe über UART");lv_obj_set_pos(u,250,200);lv_obj_set_style_text_font(u,&ui_font_de_16,0);if(DebugLog::uartEnabled())lv_obj_add_state(u,LV_STATE_CHECKED);lv_obj_add_event_cb(u,outOption,LV_EVENT_VALUE_CHANGED,(void*)2);
  lv_obj_t *er=makeButton(box,"ENERGIE LÖSCHEN",20,250,230,50,blue(),&ui_font_de_16);lv_obj_add_event_cb(er,actionEvent,LV_EVENT_CLICKED,(void*)21);lv_obj_t *r=makeButton(box,"WERKSEINSTELLUNGEN",270,250,260,50,redDark(),&ui_font_de_16);lv_obj_add_event_cb(r,actionEvent,LV_EVENT_CLICKED,(void*)20);
  touchText=makeLabel(box,"Touch-Diagnose",&ui_font_de_14,lightGrey());lv_obj_set_pos(touchText,20,320);lv_obj_set_size(touchText,720,80);
}
void updateTouchSettings(const TouchSample &s){if(!touchText)return;char text[384];snprintf(text,sizeof(text),"Touch: %s  Adresse 0x%02X  Punkte %u  RAW %u/%u  SCREEN %u/%u  I2C-Fehler R:%lu W:%lu",Touch::online()?"ONLINE":"OFFLINE",Touch::address(),s.points,s.rawX,s.rawY,s.x,s.y,(unsigned long)Touch::readErrors(),(unsigned long)Touch::writeErrors());lv_label_set_text(touchText,text);}
}
