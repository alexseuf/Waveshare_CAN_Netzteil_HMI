#include "ui_internal.h"
#include "can_driver.h"

namespace UiInternal {
constexpr int CAN_VISIBLE_ROWS = 18;
static lv_obj_t *canRows[CAN_VISIBLE_ROWS]{};
static lv_obj_t *canStats=nullptr,*pauseLabel=nullptr;
static void control(lv_event_t *e){int id=static_cast<int>(reinterpret_cast<intptr_t>(lv_event_get_user_data(e)));if(id==1){canPaused=!canPaused;lv_label_set_text(pauseLabel,canPaused?"FORTSETZEN":"PAUSE");}else{CanDriver::clearLog();for(auto *r:canRows)if(r)lv_label_set_text(r,"");}}
void makeCan(lv_obj_t *parent){
  lv_obj_t *box=lv_obj_create(parent);lv_obj_set_size(box,748,344);stylePanel(box);
  makeLabel(box,"CAN-LOGGER",&lv_font_montserrat_20,orange());
  lv_obj_t *p=makeButton(box,"PAUSE",466,1,118,34,orange(),&lv_font_montserrat_16);pauseLabel=lv_obj_get_child(p,0);lv_obj_add_event_cb(p,control,LV_EVENT_CLICKED,reinterpret_cast<void*>(1));
  lv_obj_t *c=makeButton(box,"LÖSCHEN",594,1,128,34,blue(),&ui_font_de_16);lv_obj_add_event_cb(c,control,LV_EVENT_CLICKED,reinterpret_cast<void*>(2));
  canStats=makeLabel(box,"RX 0   TX 0   Fehler 0",&ui_font_de_14,lightGrey());lv_obj_set_pos(canStats,8,34);
  const int lineH=lv_font_get_line_height(&ui_font_de_14)+2;const int firstY=55;
  for(int i=0;i<CAN_VISIBLE_ROWS;++i){canRows[i]=makeLabel(box,"",&ui_font_de_14,lv_color_white());lv_obj_set_pos(canRows[i],8,firstY+i*lineH);lv_obj_set_size(canRows[i],724,lineH);lv_label_set_long_mode(canRows[i],LV_LABEL_LONG_CLIP);}
}
void updateCan(){
  CanLogFrame frames[CAN_VISIBLE_ROWS];int count=CanDriver::copyLog(frames,CAN_VISIBLE_ROWS);char stats[96];snprintf(stats,sizeof(stats),"RX %lu   TX %lu   Fehler %lu",(unsigned long)CanDriver::rxCount(),(unsigned long)CanDriver::txCount(),(unsigned long)CanDriver::errorCount());lv_label_set_text(canStats,stats);
  for(int row=0;row<CAN_VISIBLE_ROWS;++row){if(row>=count){lv_label_set_text(canRows[row],"");continue;}const CanLogFrame &f=frames[count-1-row];char time[16];formatClock(f.ms,time,sizeof(time));char line[160];int pos=snprintf(line,sizeof(line),"%s  %s  %08lX  ",time,f.tx?"TX":"RX",(unsigned long)f.id);for(int j=0;j<f.dlc&&pos<(int)sizeof(line)-5;++j)pos+=snprintf(line+pos,sizeof(line)-pos,"%02X%s",f.data[j],j+1<f.dlc?" ":"");lv_label_set_text(canRows[row],line);lv_obj_set_style_text_color(canRows[row],f.tx?cyan():green(),0);}
}
}
