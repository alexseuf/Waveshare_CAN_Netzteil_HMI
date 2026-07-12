#include "settings.h"
#include <Preferences.h>
#include <cmath>
#include "debug_log.h"
namespace { Preferences prefs; bool ready=false, dirty=false; uint32_t dirtySince=0;
float readFloat(const char* key, float def) { if(!prefs.isKey(key)){ prefs.putFloat(key,def); DebugLog::printf("[NVS] %s neu = %.1f\n",key,def); return def;} return prefs.getFloat(key,def); }
bool valid(float v,float lo,float hi){return std::isfinite(v)&&v>=lo&&v<=hi;}
String key(char p,int i){return String(p)+String(i+1);} }
bool Settings::begin(PowerSupplyState &s){ ready=prefs.begin("psu-hmi",false); for(int i=0;i<AppConfig::PRESET_COUNT;i++){s.presets[i]={AppConfig::PRESET_DEFAULT_V[i],AppConfig::PRESET_DEFAULT_A[i]};} if(!ready)return false; s.voltageSet=readFloat("setV",AppConfig::VOLTAGE_DEFAULT_V); s.currentSet=readFloat("setA",AppConfig::CURRENT_DEFAULT_A); for(int i=0;i<AppConfig::PRESET_COUNT;i++){float v=readFloat(key('v',i).c_str(),AppConfig::PRESET_DEFAULT_V[i]);float a=readFloat(key('a',i).c_str(),AppConfig::PRESET_DEFAULT_A[i]);if(valid(v,0,58.4))s.presets[i].voltage=v;if(valid(a,0,20))s.presets[i].current=a;} s.clampSetpoints(); DebugLog::printf("[NVS] Soll %.1f V / %.1f A\n",s.voltageSet,s.currentSet); return true;}
void Settings::markDirty(){dirty=true;dirtySince=millis();}
void Settings::task(PowerSupplyState&s){if(ready&&dirty&&millis()-dirtySince>=AppConfig::SETTINGS_SAVE_DELAY_MS)saveNow(s);}
void Settings::saveNow(const PowerSupplyState&s){if(!ready)return;prefs.putFloat("setV",s.voltageSet);prefs.putFloat("setA",s.currentSet);dirty=false;DebugLog::printf("[NVS] gespeichert %.1f V / %.1f A\n",s.voltageSet,s.currentSet);}
void Settings::savePreset(PowerSupplyState&s,int i){if(i<0||i>=3)return;s.presets[i]={s.voltageSet,s.currentSet};if(ready){prefs.putFloat(key('v',i).c_str(),s.voltageSet);prefs.putFloat(key('a',i).c_str(),s.currentSet);}DebugLog::printf("[NVS] P%d gespeichert %.1f V / %.1f A\n",i+1,s.voltageSet,s.currentSet);}
void Settings::loadPreset(PowerSupplyState&s,int i){if(i<0||i>=3)return;s.voltageSet=s.presets[i].voltage;s.currentSet=s.presets[i].current;s.clampSetpoints();markDirty();DebugLog::printf("[NVS] P%d geladen %.1f V / %.1f A\n",i+1,s.voltageSet,s.currentSet);}
void Settings::resetDefaults(PowerSupplyState&s){s.voltageSet=53.3f;s.currentSet=10.0f;for(int i=0;i<3;i++)s.presets[i]={AppConfig::PRESET_DEFAULT_V[i],AppConfig::PRESET_DEFAULT_A[i]};if(ready){prefs.clear();}begin(s);DebugLog::println("[NVS] Werkseinstellungen geladen");}
