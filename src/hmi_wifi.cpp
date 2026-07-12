#include "hmi_wifi.h"
#include "debug_log.h"
#include <WiFi.h>
#include <Preferences.h>
#include <time.h>
#include <cstring>

namespace {
Preferences prefs;
bool prefsReady=false;
bool wifiEnabled=true;
bool wantConnect=false;
bool apActive=false;
bool ntpConfigured=false;
String savedSsid;
String savedPassword;
uint32_t connectStartedMs=0;
uint32_t lastReconnectMs=0;
uint32_t disconnectedSinceMs=0;
uint32_t lastNtpSyncMs=0;
uint32_t lastApAttemptMs=0;
uint8_t apAttemptCount=0;
constexpr uint32_t CONNECT_TIMEOUT_MS=15000;
constexpr uint32_t RECONNECT_INTERVAL_MS=30000;
constexpr uint32_t AP_FALLBACK_DELAY_MS=120000;
constexpr uint32_t AP_RETRY_INTERVAL_MS=60000;
constexpr uint8_t AP_MAX_ATTEMPTS=3;
constexpr const char *AP_SSID="Charger-HMI-Setup";
constexpr const char *TZ_RULE="CET-1CEST,M3.5.0/2,M10.5.0/3";
constexpr int MAX_SCAN_RESULTS=20;

struct ScanEntry { char ssid[33]; int32_t rssi; bool encrypted; };
portMUX_TYPE scanMux=portMUX_INITIALIZER_UNLOCKED;
volatile bool scanRequested=false;
volatile bool scanRunningFlag=false;
volatile bool scanReadyFlag=false;
int cachedScanCount=0;
ScanEntry cachedScan[MAX_SCAN_RESULTS]{};

void loadConfig(){if(!prefsReady)return;wifiEnabled=prefs.getBool("enabled",true);savedSsid=prefs.getString("ssid","");savedPassword=prefs.getString("pass","");}
void saveConfig(){if(!prefsReady)return;prefs.putBool("enabled",wifiEnabled);prefs.putString("ssid",savedSsid);prefs.putString("pass",savedPassword);}

void startStationConnect(){
  if(!wifiEnabled||savedSsid.isEmpty())return;
  WiFi.mode(apActive?WIFI_AP_STA:WIFI_STA);
  WiFi.setAutoReconnect(false);WiFi.persistent(false);
  WiFi.begin(savedSsid.c_str(),savedPassword.c_str());
  wantConnect=true;connectStartedMs=millis();lastReconnectMs=connectStartedMs;
  if(disconnectedSinceMs==0)disconnectedSinceMs=connectStartedMs;
  DebugLog::printf("[WIFI] Verbindung zu %s gestartet\n",savedSsid.c_str());
}

void configureNtp(){if(ntpConfigured||WiFi.status()!=WL_CONNECTED)return;configTzTime(TZ_RULE,"pool.ntp.org","time.cloudflare.com","time.google.com");ntpConfigured=true;DebugLog::println("[NTP] Synchronisation gestartet");}
void updateNtpState(){if(!ntpConfigured)return;struct tm info;if(getLocalTime(&info,0)){const time_t now=time(nullptr);if(now>1700000000){if(lastNtpSyncMs==0)DebugLog::println("[NTP] Zeit synchronisiert");lastNtpSyncMs=millis();}}}

void publishScanResults(int count){
  ScanEntry temporary[MAX_SCAN_RESULTS]{};
  const int limited=count>MAX_SCAN_RESULTS?MAX_SCAN_RESULTS:count;
  int valid=0;
  for(int i=0;i<limited;++i){
    const String name=WiFi.SSID(i);if(name.isEmpty())continue;
    strlcpy(temporary[valid].ssid,name.c_str(),sizeof(temporary[valid].ssid));
    temporary[valid].rssi=WiFi.RSSI(i);
    temporary[valid].encrypted=WiFi.encryptionType(i)!=WIFI_AUTH_OPEN;
    ++valid;
  }
  portENTER_CRITICAL(&scanMux);
  cachedScanCount=valid;memcpy(cachedScan,temporary,sizeof(cachedScan));
  scanRunningFlag=false;scanReadyFlag=true;
  portEXIT_CRITICAL(&scanMux);
  WiFi.scanDelete();
  DebugLog::printf("[WIFI] Netzwerksuche fertig: %d Netze, %d gespeichert\n",count,valid);
}
}

void HmiWifi::begin(){
  prefsReady=prefs.begin("network",false);loadConfig();WiFi.persistent(false);WiFi.setSleep(true);
  apAttemptCount=0;lastApAttemptMs=0;
  if(wifiEnabled&&!savedSsid.isEmpty())startStationConnect();
  else{WiFi.mode(WIFI_STA);disconnectedSinceMs=millis();}
}

void HmiWifi::task(){
  const uint32_t now=millis();
  bool doScan=false;
  portENTER_CRITICAL(&scanMux);
  if(scanRequested&&!scanRunningFlag){scanRequested=false;scanRunningFlag=true;scanReadyFlag=false;doScan=true;}
  portEXIT_CRITICAL(&scanMux);

  if(doScan){
    const bool reconnectAfterScan=wantConnect;
    if(apActive)WiFi.mode(WIFI_AP_STA);else WiFi.mode(WIFI_STA);
    WiFi.scanDelete();
    DebugLog::println("[WIFI] Synchrone Netzwerksuche gestartet");
    const int result=WiFi.scanNetworks(false,true,false,300);
    if(result>=0)publishScanResults(result);
    else{
      portENTER_CRITICAL(&scanMux);cachedScanCount=0;scanRunningFlag=false;scanReadyFlag=true;portEXIT_CRITICAL(&scanMux);
      DebugLog::printf("[WIFI] Netzwerksuche fehlgeschlagen, Code %d\n",result);
    }
    if(reconnectAfterScan&&!savedSsid.isEmpty()&&WiFi.status()!=WL_CONNECTED)startStationConnect();
  }

  if(!wifiEnabled)return;
  if(WiFi.status()==WL_CONNECTED){wantConnect=false;disconnectedSinceMs=0;apAttemptCount=0;if(apActive)stopAccessPoint();configureNtp();updateNtpState();return;}
  if(disconnectedSinceMs==0)disconnectedSinceMs=now;
  if(wantConnect&&now-connectStartedMs>=CONNECT_TIMEOUT_MS){wantConnect=false;WiFi.disconnect(false,false);DebugLog::println("[WIFI] Verbindungsversuch ohne Erfolg beendet");}
  if(!wantConnect&&!savedSsid.isEmpty()&&now-lastReconnectMs>=RECONNECT_INTERVAL_MS)startStationConnect();
  if(!apActive&&now-disconnectedSinceMs>=AP_FALLBACK_DELAY_MS&&apAttemptCount<AP_MAX_ATTEMPTS&&(lastApAttemptMs==0||now-lastApAttemptMs>=AP_RETRY_INTERVAL_MS)){
    lastApAttemptMs=now;++apAttemptCount;startAccessPoint();
    if(!apActive&&apAttemptCount>=AP_MAX_ATTEMPTS)DebugLog::println("[WIFI] Access-Point-Fallback nach 3 Fehlversuchen gestoppt");
  }
}

bool HmiWifi::enabled(){return wifiEnabled;}
void HmiWifi::setEnabled(bool enabled){wifiEnabled=enabled;saveConfig();if(enabled){disconnectedSinceMs=millis();apAttemptCount=0;lastApAttemptMs=0;if(!savedSsid.isEmpty())startStationConnect();}else{wantConnect=false;stopAccessPoint();WiFi.disconnect(true,false);WiFi.mode(WIFI_OFF);}}
bool HmiWifi::connected(){return WiFi.status()==WL_CONNECTED;}
bool HmiWifi::connecting(){return wantConnect;}
bool HmiWifi::accessPointActive(){return apActive;}
bool HmiWifi::timeValid(){return time(nullptr)>1700000000;}
String HmiWifi::ssid(){return connected()?WiFi.SSID():savedSsid;}
String HmiWifi::statusText(){if(connected())return "Verbunden";if(connecting())return "Verbindung läuft";if(apActive)return "Access Point aktiv";if(!wifiEnabled)return "Deaktiviert";return "Nicht verbunden";}
String HmiWifi::ipText(){return connected()?WiFi.localIP().toString():(apActive?WiFi.softAPIP().toString():String("-"));}
String HmiWifi::gatewayText(){return connected()?WiFi.gatewayIP().toString():String("-");}
String HmiWifi::macText(){return WiFi.macAddress();}
int32_t HmiWifi::rssi(){return connected()?WiFi.RSSI():-127;}
uint8_t HmiWifi::signalPercent(){const int32_t value=rssi();if(value<=-100)return 0;if(value>=-50)return 100;return static_cast<uint8_t>(2*(value+100));}
String HmiWifi::localTimeText(){struct tm info;if(!getLocalTime(&info,0))return "-";char b[32];strftime(b,sizeof(b),"%d.%m.%Y %H:%M:%S",&info);return String(b);}
String HmiWifi::lastSyncText(){if(!timeValid())return "-";struct tm info;time_t now=time(nullptr);localtime_r(&now,&info);char b[24];strftime(b,sizeof(b),"%d.%m.%Y %H:%M",&info);return String(b);}

void HmiWifi::requestScan(){portENTER_CRITICAL(&scanMux);scanRequested=true;scanReadyFlag=false;portEXIT_CRITICAL(&scanMux);}
bool HmiWifi::scanRunning(){portENTER_CRITICAL(&scanMux);const bool value=scanRequested||scanRunningFlag;portEXIT_CRITICAL(&scanMux);return value;}
bool HmiWifi::scanReady(){portENTER_CRITICAL(&scanMux);const bool value=scanReadyFlag;portEXIT_CRITICAL(&scanMux);return value;}
int HmiWifi::scanCount(){portENTER_CRITICAL(&scanMux);const int value=cachedScanCount;portEXIT_CRITICAL(&scanMux);return value;}
String HmiWifi::scanSsid(int index){char name[33]={0};portENTER_CRITICAL(&scanMux);if(index>=0&&index<cachedScanCount)strlcpy(name,cachedScan[index].ssid,sizeof(name));portEXIT_CRITICAL(&scanMux);return String(name);}
int32_t HmiWifi::scanRssi(int index){int32_t value=-127;portENTER_CRITICAL(&scanMux);if(index>=0&&index<cachedScanCount)value=cachedScan[index].rssi;portEXIT_CRITICAL(&scanMux);return value;}
bool HmiWifi::scanEncrypted(int index){bool value=false;portENTER_CRITICAL(&scanMux);if(index>=0&&index<cachedScanCount)value=cachedScan[index].encrypted;portEXIT_CRITICAL(&scanMux);return value;}

bool HmiWifi::connectTo(const String &newSsid,const String &password){if(newSsid.isEmpty())return false;savedSsid=newSsid;savedPassword=password;wifiEnabled=true;saveConfig();stopAccessPoint();WiFi.disconnect(false,false);apAttemptCount=0;lastApAttemptMs=0;disconnectedSinceMs=millis();startStationConnect();return true;}
void HmiWifi::disconnect(){wantConnect=false;WiFi.disconnect(false,false);disconnectedSinceMs=millis();apAttemptCount=0;lastApAttemptMs=0;}
void HmiWifi::startAccessPoint(){if(apActive)return;WiFi.mode(WIFI_AP_STA);const bool started=WiFi.softAP(AP_SSID);apActive=started;DebugLog::printf("[WIFI] Access Point %s: %s (Versuch %u/%u)\n",AP_SSID,started?"aktiv":"Fehler",static_cast<unsigned>(apAttemptCount),static_cast<unsigned>(AP_MAX_ATTEMPTS));}
void HmiWifi::stopAccessPoint(){if(!apActive)return;WiFi.softAPdisconnect(true);apActive=false;}
