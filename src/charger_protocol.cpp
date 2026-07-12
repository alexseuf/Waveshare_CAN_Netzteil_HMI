#include "charger_protocol.h"
#include "app_config.h"
#include <cmath>

void ChargerProtocol::buildCommand(const PowerSupplyState &state, twai_message_t &m) {
  memset(&m, 0, sizeof(m));
  m.identifier = AppConfig::CAN_ID_COMMAND;
  m.extd = 1;
  m.data_length_code = 8;
  const uint16_t voltage = static_cast<uint16_t>(lroundf(state.voltageSet * 10.0f));
  const uint16_t current = static_cast<uint16_t>(lroundf(state.currentSet * 10.0f));
  m.data[0] = static_cast<uint8_t>(voltage >> 8);
  m.data[1] = static_cast<uint8_t>(voltage);
  m.data[2] = static_cast<uint8_t>(current >> 8);
  m.data[3] = static_cast<uint8_t>(current);
  m.data[4] = state.chargingEnabled ? 0x00 : 0x01;
}

bool ChargerProtocol::decodeStatus(const twai_message_t &m, PowerSupplyState &state) {
  if (!m.extd || m.identifier != AppConfig::CAN_ID_STATUS || m.data_length_code < 5) return false;
  const uint32_t now = millis();
  const bool wasOffline = state.lastRxMs == 0 ||
                          (now - state.lastRxMs) >= AppConfig::RX_TIMEOUT_MS ||
                          state.linkState == CanLinkState::Offline ||
                          state.linkState == CanLinkState::BusOff;
  state.voltageOut = static_cast<float>((static_cast<uint16_t>(m.data[0]) << 8) | m.data[1]) * 0.1f;
  state.currentOut = static_cast<float>((static_cast<uint16_t>(m.data[2]) << 8) | m.data[3]) * 0.1f;
  state.status = m.data[4];
  state.lastRxMs = now;
  state.validRxFrames++;

  if (wasOffline) {
    state.linkState = CanLinkState::Recovering;
    state.recoveryGoodFrames = 0;
  }

  // The charger may continue to report bit 4 for one or more status cycles
  // after the physical bus is back. Keep sending commands in Recovering and
  // declare Online after three consecutive status frames with bit 4 cleared.
  if (state.communicationFault()) {
    state.linkState = CanLinkState::Recovering;
    state.recoveryGoodFrames = 0;
  } else {
    if (state.recoveryGoodFrames < 3) state.recoveryGoodFrames++;
    if (state.recoveryGoodFrames >= 3) state.linkState = CanLinkState::Online;
  }
  return true;
}

const char *ChargerProtocol::statusSummary(uint8_t status) {
  if (status & (1U << 0)) return "Hardwarefehler";
  if (status & (1U << 1)) return "Uebertemperatur";
  if (status & (1U << 2)) return "Netzspannungsfehler";
  if (status & (1U << 3)) return "Batterie fehlt/verpolt";
  if (status & (1U << 4)) return "Kommunikations-Timeout";
  return "OK";
}
