#pragma once
#include <Arduino.h>
namespace Display {
bool begin();
void task();
uint16_t fps();
}