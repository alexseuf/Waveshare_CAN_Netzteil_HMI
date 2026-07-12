#pragma once
#include <Arduino.h>
#include "driver/twai.h"
#include "power_supply.h"

namespace ChargerProtocol {
void buildCommand(const PowerSupplyState &state, twai_message_t &message);
bool decodeStatus(const twai_message_t &message, PowerSupplyState &state);
const char *statusSummary(uint8_t status);
}
