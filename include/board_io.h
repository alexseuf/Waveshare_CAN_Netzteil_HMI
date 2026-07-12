#pragma once
namespace BoardIO {
bool begin();
bool selectCanMode();
bool selectUsbMode();
void setTouchReset(bool high);
void prepareTouchAddressPin(bool high);
void releaseTouchInterruptPin();
void setBacklight(bool on);
void scanI2c();
unsigned char exioState();
}
