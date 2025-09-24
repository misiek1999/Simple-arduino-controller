#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

#include <Arduino.h>

// Joystick pin definitions
constexpr int JOYSTICK_X_PIN = A0;      /// < Joystick X-axis analog pin
constexpr int JOYSTICK_Y_PIN = A1;      /// < Joystick Y-axis analog pin
constexpr int JOYSTICK_BUTTON_PIN = 8;  /// < Joystick center button pin

// Button pin definitions
constexpr int A_PIN = 2;                /// < Button A pin
constexpr int B_PIN = 3;                /// < Button B pin
constexpr int C_PIN = 4;                /// < Button C pin
constexpr int D_PIN = 5;                /// < Button D pin
constexpr int E_PIN = 6;                /// < Button E pin
constexpr int F_PIN = 7;                /// < Button F pin

#endif
