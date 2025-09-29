#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

#include <Arduino.h>

// Serial is using pins 0 (RX) and 1 (TX), so avoid using them for other purposes

// Joystick pin definitions
constexpr int JOYSTICK_X_PIN = A0;          /// < Joystick X-axis analog pin
constexpr int JOYSTICK_Y_PIN = A1;          /// < Joystick Y-axis analog pin
constexpr int JOYSTICK_BUTTON_PIN = 8;      /// < Joystick center button pin

// Button pin definitions
constexpr int A_PIN = 2;                    /// < Button A pin
constexpr int B_PIN = 3;                    /// < Button B pin
constexpr int C_PIN = 4;                    /// < Button C pin
constexpr int D_PIN = 5;                    /// < Button D pin
constexpr int E_PIN = 6;                    /// < Button E pin
constexpr int F_PIN = 7;                    /// < Button F pin

constexpr int NRF24L01_CE_PIN   = 9;        /// < nRF24L01 CE pin
constexpr int NRF24L01_CSN_PIN  = 10;       /// < nRF24L01 CSN pin
constexpr int NRF24L01_MOSI_PIN = 11;       /// < nRF24L01 MOSI pin
constexpr int NRF24L01_MISO_PIN = 12;       /// < nRF24L01 MISO pin
constexpr int NRF24L01_SCK_PIN  = 13;       /// < nRF24L01 SCK pin

constexpr int VOLTAGE_MONITOR_PIN = A2;     /// < Voltage monitor pin (if used)

#endif
