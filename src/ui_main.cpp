#include "ui_internal.h"
#include "settings.h"
#include "app_config.h"

namespace UiInternal {
static lv_obj_t *vSet=nullptr,*aSet=nullptr,*vOut=nullptr,*aOut=nullptr,*power=nullptr;
static lv_obj_t *onlineLed=nullptr,*batLed=nullptr,*chargeLed=nullptr,*faultLed[4]{};
static lv_obj_t *presetLbl[3]{},*modeLabel=nullptr,*energyLabel=nullptr,*canSummary=nullptr;

static lv_obj_t *controlButton(lv_obj_t *parent,bool start,int x,int y,int w,int h){
  lv_obj_t *btn=makeButton(parent,"",x,y,w,h,start?greenDark():redDark(),&ui_font_de_24);
  lv_obj_t *group=lv_obj_create(btn);lv_obj_remove_style_all(group);lv_obj_set_size(group,205,44);lv_obj_center(group);
  lv_obj_clear_flag(group,LV_OBJ_FLAG_CLICKABLE);lv_obj_clear_flag(group,LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_flow(group,LV_FLEX_FLOW_ROW);lv_obj_set_flex_align(group,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);lv_obj_set_style_pad_column(group,18,0);
  lv_obj_t *icon=lv_label_create(group);lv_label_set_text(icon,start?LV_SYMBOL_PLAY:LV_SYMBOL_STOP);lv_obj_set_style_text_color(icon,lv_color_white(),0);lv_obj_set_style_text_font(icon,&lv_font_montserrat_32,0);lv_obj_clear_flag(icon,LV_OBJ_FLAG_CLICKABLE);
  lv_obj_t *txt=makeLabel(group,start?"START":"STOP",&ui_font_de_24,lv_color_white());lv_obj_clear_flag(txt,LV_OBJ_FLAG_CLICKABLE);return btn;
}
static void setPreset(lv_obj_t *obj,int index,float voltage,float current){char v[12],a[12],t[48];dtostrf(voltage,1,1,v);dtostrf(current,1,1,a);snprintf(t,sizeof(t),"P%d\n%s V\n%s A",index+1,v,a);lv_label_set_text(obj,t);lv_obj_set_style_text_align(obj,LV_TEXT_ALIGN_CENTER,0);}
static void makeSetpoint(lv_obj_t *parent,int x,int w,const char *title,const char *range,lv_obj_t **value,bool voltage){
  lv_obj_t *box=lv_obj_create(parent);lv_obj_set_pos(box,x,0);lv_obj_set_size(box,w,125);stylePanel(box);
  makeLabel(box,title,&lv_font_montserrat_20,blue());*value=makeLabel(box,voltage?"53.3 V":"10.0 A",&lv_font_montserrat_48);lv_obj_set_pos(*value,8,34);lv_obj_set_width(*value,voltage?155:128);
  lv_obj_t *r=makeLabel(box,range,&ui_font_de_14,lv_color_white());lv_obj_set_pos(r,7,96);
  int minusX=voltage?160:132;int plusX=voltage?224:196;
  lv_obj_t *minus=makeButton(box,"-",minusX,17,60,70,blue(),&lv_font_montserrat_32);lv_obj_t *plus=makeButton(box,"+",plusX,17,60,70,blue(),&lv_font_montserrat_32);
  lv_obj_add_event_cb(minus,actionEvent,LV_EVENT_CLICKED,reinterpret_cast<void*>(voltage?2:4));lv_obj_add_event_cb(minus,actionEvent,LV_EVENT_LONG_PRESSED_REPEAT,reinterpret_cast<void*>(voltage?2:4));
  lv_obj_add_event_cb(plus,actionEvent,LV_EVENT_CLICKED,reinterpret_cast<void*>(voltage?1:3));lv_obj_add_event_cb(plus,actionEvent,LV_EVENT_LONG_PRESSED_REPEAT,reinterpret_cast<void*>(voltage?1:3));
}
static void makeValue(lv_obj_t *parent,int x,int w,const char *title,lv_obj_t **value,const char *initial){lv_obj_t *box=lv_obj_create(parent);lv_obj_set_pos(box,x,133);lv_obj_set_size(box,w,105);stylePanel(box);makeLabel(box,title,&lv_font_montserrat_18,blue());*value=makeLabel(box,initial,&lv_font_montserrat_40);lv_obj_align(*value,LV_ALIGN_BOTTOM_MID,0,-8);}

void makeMain(lv_obj_t *parent){
  makeSetpoint(parent,0,292,"SPANNUNG SOLL","Einstellbereich: 51,2 - 58,4 V",&vSet,true);makeSetpoint(parent,300,266,"STROM SOLL","Einstellbereich: 0,0 - 20,0 A",&aSet,false);
  makeValue(parent,0,180,"SPANNUNG IST",&vOut,"-");makeValue(parent,188,175,"STROM IST",&aOut,"-");makeValue(parent,371,195,"LEISTUNG",&power,"-");
  lv_obj_t *presets=lv_obj_create(parent);lv_obj_set_pos(presets,0,246);lv_obj_set_size(presets,340,100);stylePanel(presets);
  makeLabel(presets,"PRESETS",&lv_font_montserrat_18,blue());lv_obj_t *hint=makeLabel(presets,"Kurz: Laden\nLang: Speichern",&ui_font_de_14,lightGrey());lv_obj_set_pos(hint,2,39);
  lv_obj_t *buttonArea=lv_obj_create(presets);lv_obj_remove_style_all(buttonArea);lv_obj_set_pos(buttonArea,82,4);lv_obj_set_size(buttonArea,246,82);lv_obj_set_flex_flow(buttonArea,LV_FLEX_FLOW_ROW);lv_obj_set_flex_align(buttonArea,LV_FLEX_ALIGN_SPACE_EVENLY,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);
  for(int i=0;i<3;++i){lv_obj_t *pb=makeButton(buttonArea,"",0,0,72,76,blue(),&ui_font_de_18);presetLbl[i]=lv_obj_get_child(pb,0);lv_obj_center(presetLbl[i]);lv_obj_add_event_cb(pb,presetEvent,LV_EVENT_CLICKED,reinterpret_cast<void*>(static_cast<intptr_t>(i)));lv_obj_add_event_cb(pb,presetEvent,LV_EVENT_LONG_PRESSED,reinterpret_cast<void*>(static_cast<intptr_t>(i)));}
  lv_obj_t *mode=lv_obj_create(parent);lv_obj_set_pos(mode,348,246);lv_obj_set_size(mode,218,100);stylePanel(mode);makeLabel(mode,"CC / CV",&lv_font_montserrat_18,blue());
  modeLabel=makeLabel(mode,"AUS",&lv_font_montserrat_32,lv_color_white());lv_obj_set_pos(modeLabel,8,42);lv_obj_set_width(modeLabel,82);lv_obj_set_style_text_align(modeLabel,LV_TEXT_ALIGN_CENTER,0);
  lv_obj_t *divider=lv_obj_create(mode);lv_obj_remove_style_all(divider);lv_obj_set_pos(divider,96,25);lv_obj_set_size(divider,2,64);lv_obj_set_style_bg_color(divider,border(),0);lv_obj_set_style_bg_opa(divider,LV_OPA_COVER,0);
  lv_obj_t *et=makeLabel(mode,"ENERGIE",&ui_font_de_14,blue());lv_obj_set_pos(et,114,3);energyLabel=makeLabel(mode,"0.000\nkWh",&ui_font_de_24,lv_color_white());lv_obj_set_pos(energyLabel,101,31);lv_obj_set_width(energyLabel,108);lv_obj_set_style_text_align(energyLabel,LV_TEXT_ALIGN_CENTER,0);
  lv_obj_t *controls=lv_obj_create(parent);lv_obj_remove_style_all(controls);lv_obj_set_pos(controls,0,354);lv_obj_set_size(controls,566,68);
  lv_obj_t *start=controlButton(controls,true,0,0,275,64);lv_obj_t *stop=controlButton(controls,false,291,0,275,64);lv_obj_add_event_cb(start,actionEvent,LV_EVENT_PRESSED,reinterpret_cast<void*>(5));lv_obj_add_event_cb(stop,actionEvent,LV_EVENT_PRESSED,reinterpret_cast<void*>(6));
  lv_obj_t *status=lv_obj_create(parent);lv_obj_set_pos(status,574,0);lv_obj_set_size(status,206,346);stylePanel(status);lv_obj_t *title=makeLabel(status,"SYSTEMSTATUS",&lv_font_montserrat_18,green());lv_obj_align(title,LV_ALIGN_TOP_MID,0,0);
  const char *sys[]={"Ladegerät online","Batterie erkannt","Laden aktiv"};lv_obj_t **leds[]={&onlineLed,&batLed,&chargeLed};for(int i=0;i<3;++i){int y=43+i*38;*leds[i]=makeLed(status,7,y,17);lv_obj_t *t=makeLabel(status,sys[i],&ui_font_de_14);lv_obj_set_pos(t,33,y-1);}
  lv_obj_t *line=lv_obj_create(status);lv_obj_remove_style_all(line);lv_obj_set_pos(line,8,159);lv_obj_set_size(line,174,2);lv_obj_set_style_bg_color(line,border(),0);lv_obj_set_style_bg_opa(line,LV_OPA_COVER,0);
  title=makeLabel(status,"FEHLER",&lv_font_montserrat_18,blue());lv_obj_set_pos(title,62,170);const char *faults[]={"Hardwarefehler","Übertemperatur","Netzspannungsfehler","Kommunikationsfehler"};for(int i=0;i<4;++i){int y=209+i*31;faultLed[i]=makeLed(status,7,y,15);lv_obj_t *t=makeLabel(status,faults[i],&ui_font_de_14);lv_obj_set_pos(t,31,y-2);}
  lv_obj_t *summary=lv_obj_create(parent);lv_obj_set_pos(summary,574,354);lv_obj_set_size(summary,206,68);stylePanel(summary);canSummary=makeLabel(summary,"CAN OFFLINE\n250 kbit/s",&ui_font_de_14,lightGrey());lv_obj_center(canSummary);lv_obj_set_style_text_align(canSummary,LV_TEXT_ALIGN_CENTER,0);
}

void updateMain(const PowerSupplyState &s,uint32_t now){
  setFloat(vSet,s.voltageSet,1,"V");setFloat(aSet,s.currentSet,1,"A");
  const bool online=s.online(now);
  if(online){setFloat(vOut,s.voltageOut,2,"V");setFloat(aOut,s.currentOut,2,"A");setFloat(power,s.outputPowerW(),1,"W");}else{lv_label_set_text(vOut,"-");lv_label_set_text(aOut,"-");lv_label_set_text(power,"-");}
  setLed(onlineLed,online);setLed(batLed,online&&!s.batteryFault());setLed(chargeLed,s.measuredCharging(now));setLed(faultLed[0],s.hardwareFault(),true);setLed(faultLed[1],s.temperatureFault(),true);setLed(faultLed[2],s.mainsFault(),true);setLed(faultLed[3],!online||s.communicationFault(),true);
  for(int i=0;i<3;++i)setPreset(presetLbl[i],i,s.presets[i].voltage,s.presets[i].current);
  ChargeMode m=s.chargeMode(now);const char *text="AUS";lv_color_t color=grey();if(m==ChargeMode::CC){text="CC";color=orange();}else if(m==ChargeMode::CV){text="CV";color=green();}lv_label_set_text(modeLabel,text);lv_obj_set_style_text_color(modeLabel,color,0);
  char val[24],out[32];dtostrf(static_cast<float>(s.energyKWh),1,s.energyKWh<10.0?3:2,val);snprintf(out,sizeof(out),"%s\nkWh",val);lv_label_set_text(energyLabel,out);
  const char *linkText=online?"ONLINE":(s.linkState==CanLinkState::BusOff?"BUS-OFF":(s.linkState==CanLinkState::Recovering?"RECOVERING":"OFFLINE"));
  char summary[64];snprintf(summary,sizeof(summary),"CAN %s\n250 kbit/s",linkText);lv_label_set_text(canSummary,summary);lv_obj_set_style_text_color(canSummary,online?green():(s.linkState==CanLinkState::Recovering?orange():red()),0);
}
}
