#pragma once
#include <Arduino.h>
namespace AppConfig {
constexpr const char *FIRMWARE_VERSION = "9.5.0-test";
constexpr uint32_t SERIAL_BAUD = 115200;
constexpr int PIN_DEBUG_TX = 43;
constexpr int PIN_DEBUG_RX = 44;
constexpr int LCD_WIDTH = 800;
constexpr int LCD_HEIGHT = 480;
constexpr int PIN_I2C_SDA = 8;
constexpr int PIN_I2C_SCL = 9;
constexpr int PIN_TOUCH_IRQ = 4;
constexpr uint32_t I2C_FREQUENCY = 400000;
constexpr uint32_t I2C_TIMEOUT_MS = 50;
constexpr bool TOUCH_SWAP_XY = false;
constexpr bool TOUCH_INVERT_X = false;
constexpr bool TOUCH_INVERT_Y = false;
constexpr int PIN_CAN_RX = 19;
constexpr int PIN_CAN_TX = 20;
constexpr uint32_t CAN_BITRATE_KBPS = 250;
constexpr uint32_t CAN_ID_COMMAND = 0x1806E5F4;
constexpr uint32_t CAN_ID_STATUS = 0x18FF50E5;
constexpr uint32_t TX_PERIOD_MS = 1000;
constexpr uint32_t RX_TIMEOUT_MS = 2500;
constexpr uint32_t CHARGER_COMM_TIMEOUT_MS = 5000;
constexpr bool USB_SEL_CAN_LEVEL_HIGH = true;
constexpr float VOLTAGE_MIN_V = 51.2f;
constexpr float VOLTAGE_MAX_V = 58.4f;
constexpr float VOLTAGE_DEFAULT_V = 53.3f;
constexpr float CURRENT_MIN_A = 0.0f;
constexpr float CURRENT_MAX_A = 20.0f;
constexpr float CURRENT_DEFAULT_A = 10.0f;
constexpr float SETPOINT_STEP = 0.1f;
constexpr int PRESET_COUNT = 3;
constexpr float PRESET_DEFAULT_V[PRESET_COUNT] = {53.3f, 56.0f, 48.0f};
constexpr float PRESET_DEFAULT_A[PRESET_COUNT] = {10.0f, 20.0f, 5.0f};
constexpr uint32_t SETTINGS_SAVE_DELAY_MS = 1200;
constexpr uint32_t ENERGY_RESET_PAUSE_MS = 5UL * 60UL * 1000UL;
}
