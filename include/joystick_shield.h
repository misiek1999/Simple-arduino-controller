/**
 * @file JoystickShield.h
 * @brief Pin definitions and constants for the joystick shield
 */

#ifndef JOYSTICK_SHIELD_H
#define JOYSTICK_SHIELD_H

// Direction enumeration
enum class JoystickDirection {
    kLeft,
    kRight,
    kUp,
    kDown,
    kCenter,
    kUnknown
};

struct CalibrationData {
    int x_center;
    int y_center;
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    bool calibrated;
};

// Button state structure
struct ButtonStates {
    bool button_a;
    bool button_b;
    bool button_c;
    bool button_d;
    bool button_e;
    bool button_f;
    bool joystick_button;
};

// Joystick data structure
struct JoystickData {
    int x_raw;
    int y_raw;
    int x_calibrated;
    int y_calibrated;
    JoystickDirection x_direction;
    JoystickDirection y_direction;
};

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
