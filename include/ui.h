#pragma once
#include "power_supply.h"
#include "touch.h"
namespace Ui {
void begin(PowerSupplyState &state);
void update(const PowerSupplyState &state);
void updateTouchDiag(const TouchSample &sample);
void handleTouch(const TouchSample &sample);
void setPerformance(uint8_t cpuLoadPercent, uint32_t loopTimeUs);
}