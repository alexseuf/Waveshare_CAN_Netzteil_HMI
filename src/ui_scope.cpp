#include "ui_internal.h"
#include "app_config.h"
#include <cmath>

namespace UiInternal {
namespace {
constexpr int SCOPE_CAPACITY=1800,SCOPE_POINTS=180;
struct ScopeSample{uint32_t ms=0;float voltage=NAN,current=NAN,power=NAN;};
ScopeSample samples[SCOPE_CAPACITY];int scopeHead=0,scopeCount=0;uint32_t lastSampleMs=0;bool scopePaused=false;
uint32_t visibleWindowMs=600000UL;float vmax=60.0f,imax=20.0f,pmax=1000.0f;
lv_obj_t *chart=nullptr,*windowLabel=nullptr,*pauseText=nullptr,*legend=nullptr,*xLabels[5]{};
lv_chart_series_t *seriesV=nullptr,*seriesI=nullptr,*seriesP=nullptr;
bool pinch=false;float pinchLast=0;uint8_t pinchMode=0;

int scaled(float v,float m){if(!isfinite(v)||m<=0)return LV_CHART_POINT_NONE;return (int)lroundf(constrain(v/m,0.0f,1.0f)*1000.0f);} 
void updateWindowLabel(){char t[24];if(visibleWindowMs<60000)snprintf(t,sizeof(t),"%lu s",(unsigned long)(visibleWindowMs/1000));else snprintf(t,sizeof(t),"%.1f min",visibleWindowMs/60000.0f);lv_label_set_text(windowLabel,t);} 
void updateXAxis(){for(int i=0;i<5;i++){float frac=(4-i)/4.0f;uint32_t ms=(uint32_t)(visibleWindowMs*frac);char t[24];if(i==4)snprintf(t,sizeof(t),"0 s");else if(ms<60000)snprintf(t,sizeof(t),"-%lu s",(unsigned long)(ms/1000));else snprintf(t,sizeof(t),"-%.1f min",ms/60000.0f);lv_label_set_text(xLabels[i],t);}}
void clearSamples(){scopeHead=scopeCount=0;lastSampleMs=0;if(chart){lv_chart_set_all_values(chart,seriesV,LV_CHART_POINT_NONE);lv_chart_set_all_values(chart,seriesI,LV_CHART_POINT_NONE);lv_chart_set_all_values(chart,seriesP,LV_CHART_POINT_NONE);lv_chart_refresh(chart);}}
void rebuild(){if(!chart)return;lv_chart_set_all_values(chart,seriesV,LV_CHART_POINT_NONE);lv_chart_set_all_values(chart,seriesI,LV_CHART_POINT_NONE);lv_chart_set_all_values(chart,seriesP,LV_CHART_POINT_NONE);if(!scopeCount){lv_chart_refresh(chart);return;}uint32_t now=samples[(scopeHead-1+SCOPE_CAPACITY)%SCOPE_CAPACITY].ms,start=now>visibleWindowMs?now-visibleWindowMs:0;int eligible=0;for(int i=0;i<scopeCount;i++){auto&s=samples[(scopeHead-scopeCount+i+SCOPE_CAPACITY)%SCOPE_CAPACITY];if(s.ms>=start)eligible++;}int stride=max(1,(eligible+SCOPE_POINTS-1)/SCOPE_POINTS),k=0;for(int i=0;i<scopeCount;i++){auto&s=samples[(scopeHead-scopeCount+i+SCOPE_CAPACITY)%SCOPE_CAPACITY];if(s.ms<start)continue;if((k++%stride)!=0)continue;lv_chart_set_next_value(chart,seriesV,scaled(s.voltage,vmax));lv_chart_set_next_value(chart,seriesI,scaled(s.current,imax));lv_chart_set_next_value(chart,seriesP,scaled(s.power,pmax));}lv_chart_refresh(chart);}
void resetView(){visibleWindowMs=600000UL;vmax=60;imax=20;pmax=1000;lv_obj_set_pos(legend,510,100);updateWindowLabel();updateXAxis();rebuild();}
void control(lv_event_t*e){int id=(int)(intptr_t)lv_event_get_user_data(e);if(id==1){scopePaused=!scopePaused;lv_label_set_text(pauseText,scopePaused?"FORTSETZEN":"PAUSE");}else if(id==2)clearSamples();else if(id==3){visibleWindowMs=max<uint32_t>(5000,visibleWindowMs/2);updateWindowLabel();updateXAxis();rebuild();}else if(id==4){visibleWindowMs=min<uint32_t>(1800000,visibleWindowMs*2);updateWindowLabel();updateXAxis();rebuild();}else if(id==5)resetView();}
void dragLegend(lv_event_t*){lv_indev_t*ind=lv_indev_active();if(!ind)return;lv_point_t v;lv_indev_get_vect(ind,&v);lv_coord_t x=lv_obj_get_x(legend)+v.x,y=lv_obj_get_y(legend)+v.y;x=constrain(x,150,590);y=constrain(y,60,270);lv_obj_set_pos(legend,x,y);}
}

void makeScope(lv_obj_t *parent){
 lv_obj_t*box=lv_obj_create(parent);lv_obj_set_size(box,780,422);stylePanel(box);makeLabel(box,"SCOPE / TREND",&lv_font_montserrat_24,orange());
 lv_obj_t*reset=makeButton(box,"↺ STANDARD",218,2,132,40,blue(),&ui_font_de_14);lv_obj_add_event_cb(reset,control,LV_EVENT_CLICKED,(void*)5);
 lv_obj_t*minus=makeButton(box,"-",360,2,42,40,blue(),&lv_font_montserrat_24);lv_obj_add_event_cb(minus,control,LV_EVENT_CLICKED,(void*)3);
 windowLabel=makeLabel(box,"10.0 min",&ui_font_de_16,lv_color_white());lv_obj_set_pos(windowLabel,413,13);
 lv_obj_t*plus=makeButton(box,"+",482,2,42,40,blue(),&lv_font_montserrat_24);lv_obj_add_event_cb(plus,control,LV_EVENT_CLICKED,(void*)4);
 lv_obj_t*pause=makeButton(box,"PAUSE",535,2,105,40,orange(),&ui_font_de_16);pauseText=lv_obj_get_child(pause,0);lv_obj_add_event_cb(pause,control,LV_EVENT_CLICKED,(void*)1);
 lv_obj_t*clear=makeButton(box,"LÖSCHEN",650,2,110,40,blue(),&ui_font_de_16);lv_obj_add_event_cb(clear,control,LV_EVENT_CLICKED,(void*)2);
 makeLabel(box,"U [V]",&ui_font_de_14,green());lv_obj_t*i=makeLabel(box,"I [A]",&ui_font_de_14,blue());lv_obj_set_pos(i,53,0);lv_obj_t*p=makeLabel(box,"P [W]",&ui_font_de_14,orange());lv_obj_set_pos(p,102,0);
 chart=lv_chart_create(box);lv_obj_set_pos(chart,140,48);lv_obj_set_size(chart,620,322);lv_chart_set_type(chart,LV_CHART_TYPE_LINE);lv_chart_set_point_count(chart,SCOPE_POINTS);lv_chart_set_range(chart,LV_CHART_AXIS_PRIMARY_Y,0,1000);lv_obj_set_style_bg_color(chart,bg(),0);lv_obj_set_style_border_color(chart,border(),0);lv_obj_set_style_line_color(chart,border(),LV_PART_MAIN);lv_obj_set_style_size(chart,0,0,LV_PART_INDICATOR);
 seriesV=lv_chart_add_series(chart,green(),LV_CHART_AXIS_PRIMARY_Y);seriesI=lv_chart_add_series(chart,blue(),LV_CHART_AXIS_PRIMARY_Y);seriesP=lv_chart_add_series(chart,orange(),LV_CHART_AXIS_PRIMARY_Y);
 const char*vt[]={"60","45","30","15","0"};const char*it[]={"20","15","10","5","0"};const char*pt[]={"1000","750","500","250","0"};for(int n=0;n<5;n++){int y=52+n*76;auto*a=makeLabel(box,vt[n],&ui_font_de_14,green());lv_obj_set_pos(a,14,y);a=makeLabel(box,it[n],&ui_font_de_14,blue());lv_obj_set_pos(a,60,y);a=makeLabel(box,pt[n],&ui_font_de_14,orange());lv_obj_set_pos(a,100,y);} 
 for(int n=0;n<5;n++){xLabels[n]=makeLabel(box,"",&ui_font_de_14,lv_color_white());lv_obj_set_pos(xLabels[n],135+n*150,378);}lv_obj_t*zeit=makeLabel(box,"Zeit",&ui_font_de_14,lightGrey());lv_obj_set_pos(zeit,18,397);updateXAxis();
 legend=lv_obj_create(box);lv_obj_set_pos(legend,510,100);lv_obj_set_size(230,118);stylePanel(legend);lv_obj_add_flag(legend,LV_OBJ_FLAG_CLICKABLE);lv_obj_add_event_cb(legend,dragLegend,LV_EVENT_PRESSING,nullptr);makeLabel(legend,"LEGENDE",&ui_font_de_16,lv_color_white());lv_obj_t*l=makeLabel(legend,"U  Spannung\nI  Strom\nP  Leistung",&ui_font_de_14,lv_color_white());lv_obj_set_pos(l,8,28);
}
void updateScope(const PowerSupplyState&s,uint32_t now){if(!scopePaused&&now-lastSampleMs>=1000){lastSampleMs=now;auto&d=samples[scopeHead];d.ms=now;if(s.online(now)){d.voltage=s.voltageOut;d.current=s.currentOut;d.power=s.outputPowerW();}else d.voltage=d.current=d.power=NAN;scopeHead=(scopeHead+1)%SCOPE_CAPACITY;if(scopeCount<SCOPE_CAPACITY)scopeCount++;rebuild();}}
void handleScopeTouch(const TouchSample&s){if(!s.newPacket)return;if(!s.pressed||s.points<2){pinch=false;return;}float dx=s.pointX[1]-s.pointX[0],dy=s.pointY[1]-s.pointY[0];float dist=sqrtf(dx*dx+dy*dy);float cx=(s.pointX[0]+s.pointX[1])*0.5f,cy=(s.pointY[0]+s.pointY[1])*0.5f;if(!pinch){pinch=true;pinchLast=dist;if(cy>415)pinchMode=1;else if(cx<58)pinchMode=2;else if(cx<103)pinchMode=3;else if(cx<150)pinchMode=4;else pinchMode=5;return;}if(pinchLast<10)return;float r=dist/pinchLast;if(r<0.92f||r>1.08f){if(pinchMode==1||pinchMode==5)visibleWindowMs=constrain((uint32_t)(visibleWindowMs/r),5000UL,1800000UL);if(pinchMode==2||pinchMode==5)vmax=constrain(vmax/r,5.0f,100.0f);if(pinchMode==3||pinchMode==5)imax=constrain(imax/r,1.0f,40.0f);if(pinchMode==4||pinchMode==5)pmax=constrain(pmax/r,100.0f,2000.0f);pinchLast=dist;updateWindowLabel();updateXAxis();rebuild();}}
}
