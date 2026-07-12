#include "power_supply.h"
#include "debug_log.h"
#include <cmath>

void PowerSupplyState::clampSetpoints() {
  voltageSet = constrain(voltageSet, AppConfig::VOLTAGE_MIN_V, AppConfig::VOLTAGE_MAX_V);
  currentSet = constrain(currentSet, AppConfig::CURRENT_MIN_A, AppConfig::CURRENT_MAX_A);
  voltageSet = roundf(voltageSet * 10.0f) / 10.0f;
  currentSet = roundf(currentSet * 10.0f) / 10.0f;
}

bool PowerSupplyState::online(uint32_t now) const {
  return canStarted && lastRxMs != 0 && (now - lastRxMs) < AppConfig::RX_TIMEOUT_MS &&
         linkState != CanLinkState::BusOff;
}

bool PowerSupplyState::measuredCharging(uint32_t now) const {
  return chargingEnabled && chargerReady(now) && voltageOut > 1.0f && currentOut > 0.05f;
}

ChargeMode PowerSupplyState::chargeMode(uint32_t now) const {
  if (!measuredCharging(now) || voltageSet < 0.1f || currentSet < 0.05f) {
    lastRegulationMode = ChargeMode::Off;
    return ChargeMode::Off;
  }

  // Compare relative setpoint errors. The controlled quantity with the smaller
  // percentage error determines the displayed regulation mode.
  const float voltageErrorPct = fabsf(voltageOut - voltageSet) / fmaxf(voltageSet, 0.1f) * 100.0f;
  const float currentErrorPct = fabsf(currentOut - currentSet) / fmaxf(currentSet, 0.1f) * 100.0f;
  constexpr float HYSTERESIS_PERCENTAGE_POINTS = 0.25f;

  if (currentErrorPct + HYSTERESIS_PERCENTAGE_POINTS < voltageErrorPct)
    lastRegulationMode = ChargeMode::CC;
  else if (voltageErrorPct + HYSTERESIS_PERCENTAGE_POINTS < currentErrorPct)
    lastRegulationMode = ChargeMode::CV;
  else if (lastRegulationMode == ChargeMode::Off)
    lastRegulationMode = voltageErrorPct <= currentErrorPct ? ChargeMode::CV : ChargeMode::CC;

  return lastRegulationMode;
}

void PowerSupplyState::updateEnergy(uint32_t now) {
  if (energyLastUpdateMs == 0) { energyLastUpdateMs = now; return; }
  uint32_t dtMs = now - energyLastUpdateMs;
  energyLastUpdateMs = now;
  if (dtMs > 5000) dtMs = 5000;

  if (measuredCharging(now)) {
    energyKWh += static_cast<double>(outputPowerW()) * static_cast<double>(dtMs) / 3600000000.0;
    chargeInterruptedSinceMs = 0;
    energyResetDoneForPause = false;
  } else {
    if (chargeInterruptedSinceMs == 0) chargeInterruptedSinceMs = now;
    if (!energyResetDoneForPause && now - chargeInterruptedSinceMs >= AppConfig::ENERGY_RESET_PAUSE_MS) {
      resetEnergy();
      energyResetDoneForPause = true;
      DebugLog::log(DebugLog::SYSTEM, "[ENERGIE] Zähler nach 5 min Ladeunterbrechung gelöscht\n");
    }
  }
}

void PowerSupplyState::resetEnergy() { energyKWh = 0.0; }

const char *PowerSupplyState::linkStateText() const {
  switch (linkState) {
    case CanLinkState::Online: return "ONLINE";
    case CanLinkState::Recovering: return "RECOVERING";
    case CanLinkState::BusOff: return "BUS-OFF";
    default: return "OFFLINE";
  }
}
