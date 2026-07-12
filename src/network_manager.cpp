#include "network_manager.h"
#include "debug_log.h"
#include <WiFi.h>
#include <Preferences.h>
#include <time.h>

namespace {
Preferences prefs;
bool prefsReady=false;
bool wifiEnabled=true;
bool wantConnect=false;
bool apActive=false;
bool scanRequested=false;
bool scanDone=false;
bool ntpConfigured=false;
String savedSsid;
String savedPassword;
uint32_t connectStartedMs=0;
uint32_t lastReconnectMs=0;
uint32_t disconnectedSinceMs=0;
uint32_t lastNtpSyncMs=0;
constexpr uint32_t CONNECT_TIMEOUT_MS=15000;
constexpr uint32_t RECONNECT_INTERVAL_MS=30000;
constexpr uint32_t AP_FALLBACK_DELAY_MS=120000;
constexpr const char *AP_SSID="Charger-HMI-Setup";
constexpr const char *TZ_RULE="CET-1CEST,M3.5.0/2,M10.5.0/3";

void loadConfig(){
  if(!prefsReady)return;
  wifiEnabled=prefs.getBool("enabled",true);
  savedSsid=prefs.getString("ssid","");
  savedPassword=prefs.getString("pass","");
}

void saveConfig(){
  if(!prefsReady)return;
  prefs.putBool("enabled",wifiEnabled);
  prefs.putString("ssid",savedSsid);
  prefs.putString("pass",savedPassword);
}

void startStationConnect(){
  if(!wifiEnabled||savedSsid.isEmpty())return;
  WiFi.mode(apActive?WIFI_AP_STA:WIFI_STA);
  WiFi.setAutoReconnect(false);
  WiFi.persistent(false);
  WiFi.begin(savedSsid.c_str(),savedPassword.c_str());
  wantConnect=true;
  connectStartedMs=millis();
  lastReconnectMs=connectStartedMs;
  if(disconnectedSinceMs==0)disconnectedSinceMs=connectStartedMs;
  DebugLog::printf("[WIFI] Verbindung zu %s gestartet\n",savedSsid.c_str());
}

void configureNtp(){
  if(ntpConfigured||WiFi.status()!=WL_CONNECTED)return;
  configTzTime(TZ_RULE,"pool.ntp.org","time.cloudflare.com","time.google.com");
  ntpConfigured=true;
  DebugLog::println("[NTP] Synchronisation gestartet");
}

void updateNtpState(){
  if(!ntpConfigured)return;
  struct tm info;
  if(getLocalTime(&info,0)){
    const time_t now=time(nullptr);
    if(now>1700000000){
      if(lastNtpSyncMs==0)DebugLog::println("[NTP] Zeit synchronisiert");
      lastNtpSyncMs=millis();
    }
  }
}
}

void NetworkManager::begin(){
  prefsReady=prefs.begin("network",false);
  loadConfig();
  WiFi.persistent(false);
  WiFi.setSleep(true);
  if(wifiEnabled&&!savedSsid.isEmpty())startStationConnect();
  else WiFi.mode(WIFI_OFF);
}

void NetworkManager::task(){
  const uint32_t now=millis();

  if(scanRequested&&WiFi.scanComplete()!=WIFI_SCAN_RUNNING){
    WiFi.mode(apActive?WIFI_AP_STA:WIFI_STA);
    WiFi.scanDelete();
    WiFi.scanNetworks(true,true,false,300);
    scanRequested=false;
    scanDone=false;
    DebugLog::println("[WIFI] Netzwerksuche gestartet");
  }

  const int scanState=WiFi.scanComplete();
  if(scanState>=0&&!scanDone){
    scanDone=true;
    DebugLog::printf("[WIFI] Netzwerksuche fertig: %d Netze\n",scanState);
  }

  if(!wifiEnabled)return;

  if(WiFi.status()==WL_CONNECTED){
    wantConnect=false;
    disconnectedSinceMs=0;
    if(apActive)stopAccessPoint();
    configureNtp();
    updateNtpState();
    return;
  }

  if(disconnectedSinceMs==0)disconnectedSinceMs=now;

  if(wantConnect&&now-connectStartedMs>=CONNECT_TIMEOUT_MS){
    wantConnect=false;
    WiFi.disconnect(false,false);
    DebugLog::println("[WIFI] Verbindungsversuch ohne Erfolg beendet");
  }

  if(!wantConnect&&!savedSsid.isEmpty()&&now-lastReconnectMs>=RECONNECT_INTERVAL_MS){
    startStationConnect();
  }

  if(!apActive&&now-disconnectedSinceMs>=AP_FALLBACK_DELAY_MS){
    startAccessPoint();
  }
}

bool NetworkManager::enabled(){return wifiEnabled;}
void NetworkManager::setEnabled(bool enabled){
  wifiEnabled=enabled;saveConfig();
  if(enabled){if(!savedSsid.isEmpty())startStationConnect();}
  else{wantConnect=false;stopAccessPoint();WiFi.disconnect(true,false);WiFi.mode(WIFI_OFF);}
}

bool NetworkManager::connected(){return WiFi.status()==WL_CONNECTED;}
bool NetworkManager::connecting(){return wantConnect;}
bool NetworkManager::accessPointActive(){return apActive;}

bool NetworkManager::timeValid(){
  const time_t now=time(nullptr);
  return now>1700000000;
}

String NetworkManager::ssid(){return connected()?WiFi.SSID():savedSsid;}
String NetworkManager::statusText(){
  if(connected())return "Verbunden";
  if(connecting())return "Verbindung läuft";
  if(apActive)return "Access Point aktiv";
  if(!wifiEnabled)return "Deaktiviert";
  return "Nicht verbunden";
}
String NetworkManager::ipText(){return connected()?WiFi.localIP().toString():(apActive?WiFi.softAPIP().toString():String("-"));}
String NetworkManager::gatewayText(){return connected()?WiFi.gatewayIP().toString():String("-");}
String NetworkManager::macText(){return WiFi.macAddress();}
int32_t NetworkManager::rssi(){return connected()?WiFi.RSSI():-127;}
uint8_t NetworkManager::signalPercent(){
  const int32_t value=rssi();
  if(value<=-100)return 0;if(value>=-50)return 100;return static_cast<uint8_t>(2*(value+100));
}
String NetworkManager::localTimeText(){
  struct tm info;if(!getLocalTime(&info,0))return "-";char b[32];strftime(b,sizeof(b),"%d.%m.%Y %H:%M:%S",&info);return String(b);
}
String NetworkManager::lastSyncText(){
  if(!timeValid())return "-";
  struct tm info;time_t now=time(nullptr);localtime_r(&now,&info);char b[24];strftime(b,sizeof(b),"%d.%m.%Y %H:%M",&info);return String(b);
}

void NetworkManager::requestScan(){scanRequested=true;scanDone=false;}
bool NetworkManager::scanRunning(){return WiFi.scanComplete()==WIFI_SCAN_RUNNING||scanRequested;}
bool NetworkManager::scanReady(){return scanDone&&WiFi.scanComplete()>=0;}
int NetworkManager::scanCount(){const int n=WiFi.scanComplete();return n<0?0:n;}
String NetworkManager::scanSsid(int index){const int n=scanCount();return index>=0&&index<n?WiFi.SSID(index):String();}
int32_t NetworkManager::scanRssi(int index){const int n=scanCount();return index>=0&&index<n?WiFi.RSSI(index):-127;}
bool NetworkManager::scanEncrypted(int index){const int n=scanCount();return index>=0&&index<n&&WiFi.encryptionType(index)!=WIFI_AUTH_OPEN;}

bool NetworkManager::connectTo(const String &newSsid,const String &password){
  if(newSsid.isEmpty())return false;
  savedSsid=newSsid;savedPassword=password;wifiEnabled=true;saveConfig();stopAccessPoint();WiFi.disconnect(false,false);startStationConnect();return true;
}
void NetworkManager::disconnect(){wantConnect=false;WiFi.disconnect(false,false);disconnectedSinceMs=millis();}
void NetworkManager::startAccessPoint(){
  if(apActive)return;
  WiFi.mode(WIFI_AP_STA);
  apActive=WiFi.softAP(AP_SSID);
  DebugLog::printf("[WIFI] Access Point %s: %s\n",AP_SSID,apActive?"aktiv":"Fehler");
}
void NetworkManager::stopAccessPoint(){if(!apActive)return;WiFi.softAPdisconnect(true);apActive=false;}
