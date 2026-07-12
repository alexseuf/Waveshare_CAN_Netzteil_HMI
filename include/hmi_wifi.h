#pragma once
#include <Arduino.h>
#include <IPAddress.h>

namespace HmiWifi {

void begin();
void task();

bool enabled();
void setEnabled(bool enabled);

bool connected();
bool connecting();
bool accessPointActive();
bool timeValid();

String ssid();
String statusText();
String ipText();
String gatewayText();
String macText();
int32_t rssi();
uint8_t signalPercent();
String localTimeText();
String lastSyncText();

void requestScan();
bool scanRunning();
bool scanReady();
int scanCount();
String scanSsid(int index);
int32_t scanRssi(int index);
bool scanEncrypted(int index);

bool connectTo(const String &ssid, const String &password);
void disconnect();
void startAccessPoint();
void stopAccessPoint();

}