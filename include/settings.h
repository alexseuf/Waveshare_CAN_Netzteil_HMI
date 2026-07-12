#pragma once
#include "power_supply.h"
namespace Settings {
bool begin(PowerSupplyState &state);
void markDirty();
void task(PowerSupplyState &state);
void saveNow(const PowerSupplyState &state);
void savePreset(PowerSupplyState &state, int index);
void loadPreset(PowerSupplyState &state, int index);
void resetDefaults(PowerSupplyState &state);
}
