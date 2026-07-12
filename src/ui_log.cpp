#include "ui_internal.h"
namespace UiInternal {
constexpr int LOG_VISIBLE_ROWS = 18;
static lv_obj_t *logRows[LOG_VISIBLE_ROWS]{};
static lv_obj_t *pauseLabel=nullptr;
static lv_color_t categoryColor(uint16_t c){switch(c){case DebugLog::ERROR:return red();case DebugLog::WARNING:return orange();case DebugLog::CAN_RX:return green();case DebugLog::CAN_TX:return cyan();case DebugLog::UI:return blue();case DebugLog::TOUCH:return lv_color_hex(0xB06CFF);case DebugLog::STORAGE:return lightGrey();default:return lv_color_white();}}
static void control(lv_event_t *e){int id=static_cast<int>(reinterpret_cast<intptr_t>(lv_event_get_user_data(e)));if(id==1){logPaused=!logPaused;lv_label_set_text(pauseLabel,logPaused?"FORTSETZEN":"PAUSE");}else{DebugLog::clear();for(auto *r:logRows)if(r)lv_label_set_text(r,"");}}
void makeLog(lv_obj_t *parent){
  lv_obj_t *box=lv_obj_create(parent);lv_obj_set_size(box,780,422);stylePanel(box);makeLabel(box,"DEBUG-LOG",&lv_font_montserrat_24,orange());
  lv_obj_t *p=makeButton(box,"PAUSE",490,4,120,38,orange(),&lv_font_montserrat_16);pauseLabel=lv_obj_get_child(p,0);lv_obj_add_event_cb(p,control,LV_EVENT_CLICKED,reinterpret_cast<void*>(1));
  lv_obj_t *c=makeButton(box,"LÖSCHEN",620,4,130,38,blue(),&ui_font_de_16);lv_obj_add_event_cb(c,control,LV_EVENT_CLICKED,reinterpret_cast<void*>(2));
  const int lineH=lv_font_get_line_height(&ui_font_de_14)+3;const int firstY=51;
  for(int i=0;i<LOG_VISIBLE_ROWS;++i){logRows[i]=makeLabel(box,"",&ui_font_de_14,lv_color_white());lv_obj_set_pos(logRows[i],10,firstY+i*lineH);lv_obj_set_size(logRows[i],750,lineH);lv_label_set_long_mode(logRows[i],LV_LABEL_LONG_CLIP);}
}
void updateLog(){
  DebugLog::Entry entries[LOG_VISIBLE_ROWS];int count=DebugLog::copyEntries(entries,LOG_VISIBLE_ROWS);
  for(int row=0;row<LOG_VISIBLE_ROWS;++row){if(row>=count){lv_label_set_text(logRows[row],"");continue;}const DebugLog::Entry &e=entries[count-1-row];char time[16];formatClock(e.ms,time,sizeof(time));char line[230];snprintf(line,sizeof(line),"%s  %-7s  %s",time,DebugLog::categoryName(e.category),e.text);lv_label_set_text(logRows[row],line);lv_obj_set_style_text_color(logRows[row],categoryColor(e.category),0);}
}
}
