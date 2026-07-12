#include "can_driver.h"
#include "app_config.h"
#include "charger_protocol.h"
#include "debug_log.h"
#include <Arduino.h>
#include "driver/twai.h"

namespace {
constexpr int LOGN = 40;
CanLogFrame logs[LOGN];
int head = 0, count = 0;
uint32_t rxC = 0, txC = 0, errC = 0;
bool recoveryInProgress = false;

void add(bool tx, const twai_message_t &m) {
  CanLogFrame &l = logs[head];
  l.ms = millis(); l.id = m.identifier; l.tx = tx; l.extended = m.extd;
  l.dlc = m.data_length_code;
  memset(l.data, 0, sizeof(l.data));
  memcpy(l.data, m.data, m.data_length_code < 8 ? m.data_length_code : 8);
  head = (head + 1) % LOGN;
  if (count < LOGN) count++;
}
}

bool CanDriver::begin() {
  twai_general_config_t g = TWAI_GENERAL_CONFIG_DEFAULT(
      static_cast<gpio_num_t>(AppConfig::PIN_CAN_TX),
      static_cast<gpio_num_t>(AppConfig::PIN_CAN_RX), TWAI_MODE_NORMAL);
  g.tx_queue_len = 8;
  g.rx_queue_len = 32;
  g.alerts_enabled = TWAI_ALERT_BUS_ERROR | TWAI_ALERT_ERR_PASS |
                     TWAI_ALERT_BUS_OFF | TWAI_ALERT_BUS_RECOVERED |
                     TWAI_ALERT_RECOVERY_IN_PROGRESS | TWAI_ALERT_RX_QUEUE_FULL;

  twai_timing_config_t t = TWAI_TIMING_CONFIG_250KBITS();
  if (AppConfig::CAN_BITRATE_KBPS == 125) t = TWAI_TIMING_CONFIG_125KBITS();
  else if (AppConfig::CAN_BITRATE_KBPS == 500) t = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  esp_err_t e = twai_driver_install(&g, &t, &f);
  DebugLog::printf("[CAN] twai_driver_install=%s\n", esp_err_to_name(e));
  if (e != ESP_OK) { errC++; return false; }
  e = twai_start();
  DebugLog::printf("[CAN] twai_start=%s TX=%d RX=%d %lu kbit/s\n",
                   esp_err_to_name(e), AppConfig::PIN_CAN_TX, AppConfig::PIN_CAN_RX,
                   static_cast<unsigned long>(AppConfig::CAN_BITRATE_KBPS));
  if (e != ESP_OK) { errC++; return false; }
  return true;
}

bool CanDriver::sendCommand(PowerSupplyState &s) {
  if (!s.canStarted || recoveryInProgress || s.linkState == CanLinkState::BusOff) return false;
  s.clampSetpoints();
  twai_message_t m{};
  ChargerProtocol::buildCommand(s, m);
  const esp_err_t e = twai_transmit(&m, pdMS_TO_TICKS(50));
  s.lastTxMs = millis();
  if (e == ESP_OK) { txC++; add(true, m); } else errC++;
  DebugLog::log(DebugLog::CAN_TX,
      "[CAN TX] #%lu ID=0x%08lX DATA=%02X %02X %02X %02X %02X %02X %02X %02X -> %s\n",
      static_cast<unsigned long>(txC), static_cast<unsigned long>(m.identifier),
      m.data[0],m.data[1],m.data[2],m.data[3],m.data[4],m.data[5],m.data[6],m.data[7],
      esp_err_to_name(e));
  return e == ESP_OK;
}

void CanDriver::poll(PowerSupplyState &s) {
  if (!s.canStarted) return;
  const uint32_t now = millis();

  twai_message_t m{};
  while (twai_receive(&m, 0) == ESP_OK) {
    rxC++; add(false, m);
    char rxLine[220];
    int pos = snprintf(rxLine, sizeof(rxLine), "[CAN RX] #%lu ID=0x%08lX %s DLC=%u DATA=",
                       static_cast<unsigned long>(rxC), static_cast<unsigned long>(m.identifier),
                       m.extd ? "EXT" : "STD", m.data_length_code);
    for (int i=0; i<m.data_length_code && pos<(int)sizeof(rxLine)-5; ++i)
      pos += snprintf(rxLine+pos, sizeof(rxLine)-pos, "%02X%s", m.data[i], i+1<m.data_length_code?" ":"");
    snprintf(rxLine+pos, sizeof(rxLine)-pos, "\n");
    DebugLog::log(DebugLog::CAN_RX, "%s", rxLine);

    if (ChargerProtocol::decodeStatus(m, s)) {
      DebugLog::printf("[NETZTEIL] U=%.1f V I=%.1f A P=%.1f W Status=0x%02X Link=%s\n",
                       s.voltageOut, s.currentOut, s.outputPowerW(), s.status, s.linkStateText());
      if (s.linkState == CanLinkState::Recovering && now - s.lastRecoveryTxMs >= 200) {
        s.lastRecoveryTxMs = now;
        DebugLog::log(DebugLog::SYSTEM, "[CAN] Recovery: Steuertelegramm sofort senden\n");
        sendCommand(s);
      }
    }
  }

  if (s.lastRxMs && now - s.lastRxMs >= AppConfig::RX_TIMEOUT_MS &&
      s.linkState != CanLinkState::BusOff) {
    if (s.linkState != CanLinkState::Offline)
      DebugLog::log(DebugLog::WARNING, "[CAN] RX-Timeout; Verbindung OFFLINE\n");
    s.linkState = CanLinkState::Offline;
    s.recoveryGoodFrames = 0;
  }

  uint32_t alerts = 0;
  if (twai_read_alerts(&alerts, 0) == ESP_OK && alerts) {
    if (alerts & TWAI_ALERT_BUS_OFF) {
      errC++;
      s.linkState = CanLinkState::BusOff;
      recoveryInProgress = true;
      const esp_err_t e = twai_initiate_recovery();
      DebugLog::log(DebugLog::ERROR, "[CAN] BUS-OFF; Recovery gestartet: %s\n", esp_err_to_name(e));
    }
    if (alerts & TWAI_ALERT_BUS_RECOVERED) {
      const esp_err_t e = twai_start();
      recoveryInProgress = false;
      s.linkState = CanLinkState::Recovering;
      s.recoveryGoodFrames = 0;
      s.lastRxMs = 0;
      DebugLog::log(DebugLog::SYSTEM, "[CAN] Bus wiederhergestellt; twai_start=%s\n", esp_err_to_name(e));
      if (e == ESP_OK) sendCommand(s);
    }
    if (alerts & (TWAI_ALERT_BUS_ERROR | TWAI_ALERT_ERR_PASS | TWAI_ALERT_RX_QUEUE_FULL)) errC++;
    DebugLog::printf("[CAN ALERT] 0x%08lX\n", static_cast<unsigned long>(alerts));
  }
}

int CanDriver::copyLog(CanLogFrame *d, int max) {
  const int n = min(count, max);
  for (int i=0; i<n; ++i) d[i] = logs[(head - 1 - i + LOGN) % LOGN];
  return n;
}
void CanDriver::clearLog(){ head=0; count=0; }
uint32_t CanDriver::rxCount(){return rxC;}
uint32_t CanDriver::txCount(){return txC;}
uint32_t CanDriver::errorCount(){return errC;}
