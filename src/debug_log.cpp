#include "debug_log.h"
#include "app_config.h"
#include <Preferences.h>
#include <cstdarg>
#include <cstring>

namespace {
HardwareSerial debugUart(1);
Preferences prefs;
constexpr int LOG_CAPACITY = 64;
DebugLog::Entry entries[LOG_CAPACITY];
int head = 0;
int count = 0;
uint16_t mask = DebugLog::ALL;
bool showDisplay = true;
bool showUart = true;

uint16_t detectCategory(const char *text) {
  if (!text) return DebugLog::SYSTEM;
  if (strstr(text, "[CAN RX]")) return DebugLog::CAN_RX;
  if (strstr(text, "[CAN TX]")) return DebugLog::CAN_TX;
  if (strstr(text, "[UI]")) return DebugLog::UI;
  if (strstr(text, "[TOUCH]")) return DebugLog::TOUCH;
  if (strstr(text, "[NVS]")) return DebugLog::STORAGE;
  if (strstr(text, "FEHLER") || strstr(text, "[ERROR]")) return DebugLog::ERROR;
  if (strstr(text, "[WARN]") || strstr(text, "Warn")) return DebugLog::WARNING;
  return DebugLog::SYSTEM;
}

void append(uint16_t category, const char *text) {
  if (!text || !(mask & category)) return;
  DebugLog::Entry &e = entries[head];
  e.ms = millis();
  e.category = category;
  strncpy(e.text, text, sizeof(e.text) - 1);
  e.text[sizeof(e.text) - 1] = '\0';
  size_t n = strlen(e.text);
  while (n && (e.text[n - 1] == '\n' || e.text[n - 1] == '\r')) e.text[--n] = '\0';
  head = (head + 1) % LOG_CAPACITY;
  if (count < LOG_CAPACITY) count++;
}

void emit(uint16_t category, const char *text) {
  if (!text) return;
  append(category, text);
  if (showUart && (mask & category)) debugUart.print(text);
}
}

void DebugLog::begin() {
  debugUart.begin(AppConfig::SERIAL_BAUD, SERIAL_8N1,
                  AppConfig::PIN_DEBUG_RX, AppConfig::PIN_DEBUG_TX);
  delay(50);
  if (prefs.begin("debug-log", false)) {
    mask = prefs.getUShort("mask", ALL);
    showDisplay = prefs.getBool("display", true);
    showUart = prefs.getBool("uart", true);
  }
}

HardwareSerial &DebugLog::port() { return debugUart; }
void DebugLog::print(const char *text) { emit(detectCategory(text), text); }
void DebugLog::println(const char *text) {
  char buffer[140];
  snprintf(buffer, sizeof(buffer), "%s\n", text ? text : "");
  emit(detectCategory(buffer), buffer);
}
void DebugLog::printf(const char *format, ...) {
  char buffer[384];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  emit(detectCategory(buffer), buffer);
}
void DebugLog::log(uint16_t category, const char *format, ...) {
  char buffer[384];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  emit(category, buffer);
}

int DebugLog::copyEntries(Entry *dest, int maxCount) {
  if (!dest || maxCount <= 0 || !showDisplay) return 0;
  const int n = min(count, maxCount);
  for (int i = 0; i < n; ++i) dest[i] = entries[(head - 1 - i + LOG_CAPACITY) % LOG_CAPACITY];
  return n;
}
void DebugLog::clear() { head = 0; count = 0; }
uint16_t DebugLog::categoryMask() { return mask; }
void DebugLog::setCategoryMask(uint16_t value) {
  mask = value;
  prefs.putUShort("mask", mask);
}
bool DebugLog::categoryEnabled(uint16_t category) { return (mask & category) != 0; }
bool DebugLog::displayEnabled() { return showDisplay; }
void DebugLog::setDisplayEnabled(bool enabled) { showDisplay = enabled; prefs.putBool("display", enabled); }
bool DebugLog::uartEnabled() { return showUart; }
void DebugLog::setUartEnabled(bool enabled) { showUart = enabled; prefs.putBool("uart", enabled); }
const char *DebugLog::categoryName(uint16_t category) {
  switch (category) {
    case ERROR: return "FEHLER";
    case WARNING: return "WARN";
    case CAN_RX: return "CAN RX";
    case CAN_TX: return "CAN TX";
    case UI: return "UI";
    case TOUCH: return "TOUCH";
    case STORAGE: return "NVS";
    default: return "INFO";
  }
}
