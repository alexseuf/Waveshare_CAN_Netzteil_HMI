#pragma once
#include <Arduino.h>

namespace DebugLog {
enum Category : uint16_t {
  SYSTEM = 1U << 0,
  ERROR  = 1U << 1,
  WARNING= 1U << 2,
  CAN_RX = 1U << 3,
  CAN_TX = 1U << 4,
  UI     = 1U << 5,
  TOUCH  = 1U << 6,
  STORAGE= 1U << 7,
  ALL    = 0xFFFFU
};

struct Entry {
  uint32_t ms = 0;
  uint16_t category = SYSTEM;
  char text[120]{};
};

void begin();
void print(const char *text);
void println(const char *text = "");
void printf(const char *format, ...) __attribute__((format(printf, 1, 2)));
void log(uint16_t category, const char *format, ...) __attribute__((format(printf, 2, 3)));
HardwareSerial &port();

int copyEntries(Entry *dest, int maxCount);
void clear();
uint16_t categoryMask();
void setCategoryMask(uint16_t mask);
bool categoryEnabled(uint16_t category);
bool displayEnabled();
void setDisplayEnabled(bool enabled);
bool uartEnabled();
void setUartEnabled(bool enabled);
const char *categoryName(uint16_t category);
}
