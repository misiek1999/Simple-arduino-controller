/**
 * @file JoystickShield.h
 * @brief Pin definitions and constants for the joystick shield
 */

#ifndef JOYSTICK_SHIELD_H
#define JOYSTICK_SHIELD_H

#include "joystick_shield_struct.h"

// Function declarations
void JoystickShieldSetup();
void CalibrateJoystick(CalibrationData* cal_data);
void ReadJoystickData(JoystickData* data, const CalibrationData* cal_data = nullptr);
void ReadButtonStates(ButtonStates* states);
JoystickDirection GetJoystickDirection(int analog_value, int center_value, bool is_vertical = false);
const char* DirectionToString(JoystickDirection direction);
void PrintActiveButtons(const ButtonStates* states);
void PrintJoystickData(const JoystickData* data);
void PrintCalibrationData(const CalibrationData* cal_data);

#endif // JOYSTICK_SHIELD_H
