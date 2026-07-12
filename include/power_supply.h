#pragma once
#include <Arduino.h>
#include "app_config.h"

struct PowerPreset { float voltage; float current; };

enum class ChargeMode : uint8_t { Off, CC, CV };
enum class CanLinkState : uint8_t { Offline, Recovering, Online, BusOff };

struct PowerSupplyState {
  float voltageSet = AppConfig::VOLTAGE_DEFAULT_V;
  float currentSet = AppConfig::CURRENT_DEFAULT_A;
  float voltageOut = 0.0f;
  float currentOut = 0.0f;
  uint8_t status = 0;
  bool chargingEnabled = false;
  bool canStarted = false;
  uint32_t lastRxMs = 0;
  uint32_t lastTxMs = 0;
  uint32_t validRxFrames = 0;
  uint32_t lastRecoveryTxMs = 0;
  uint8_t recoveryGoodFrames = 0;
  CanLinkState linkState = CanLinkState::Offline;
  PowerPreset presets[AppConfig::PRESET_COUNT];

  double energyKWh = 0.0;
  uint32_t energyLastUpdateMs = 0;
  uint32_t chargeInterruptedSinceMs = 0;
  bool energyResetDoneForPause = false;

  // CC/CV selection hysteresis state.
  mutable ChargeMode lastRegulationMode = ChargeMode::Off;

  void clampSetpoints();
  bool online(uint32_t now) const;
  bool hardwareFault() const { return status & (1U << 0); }
  bool temperatureFault() const { return status & (1U << 1); }
  bool mainsFault() const { return status & (1U << 2); }
  bool batteryFault() const { return status & (1U << 3); }
  bool communicationFault() const { return status & (1U << 4); }
  bool batteryOk() const { return online(millis()) && !batteryFault(); }
  bool chargerReady(uint32_t now) const {
    return online(now) && !hardwareFault() && !temperatureFault() && !mainsFault() &&
           !batteryFault() && !communicationFault();
  }
  bool anyFault() const {
    return hardwareFault() || temperatureFault() || mainsFault() || communicationFault();
  }
  float outputPowerW() const { return voltageOut * currentOut; }
  bool measuredCharging(uint32_t now) const;
  ChargeMode chargeMode(uint32_t now) const;
  void updateEnergy(uint32_t now);
  void resetEnergy();
  const char *linkStateText() const;
};
