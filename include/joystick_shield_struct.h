/**
 * @file joystick_shield_struct.h
 * @brief Data structures for joystick shield
 */

#ifndef JOYSTICK_SHIELD_STRUCT_H
#define JOYSTICK_SHIELD_STRUCT_H

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

struct PadData {
    JoystickData joystick;
    ButtonStates buttons;
};

#endif // JOYSTICK_SHIELD_STRUCT_H
