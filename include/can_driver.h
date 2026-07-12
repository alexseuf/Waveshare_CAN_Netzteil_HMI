#pragma once
#include "power_supply.h"
struct CanLogFrame { uint32_t ms=0,id=0; bool tx=false,extended=false; uint8_t dlc=0,data[8]{}; };
namespace CanDriver {
bool begin();
bool sendCommand(PowerSupplyState &state);
void poll(PowerSupplyState &state);
int copyLog(CanLogFrame *dest,int maxCount);
void clearLog();
uint32_t rxCount(); uint32_t txCount(); uint32_t errorCount();
}
