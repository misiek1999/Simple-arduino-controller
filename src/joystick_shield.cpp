/**
 * @file JoystickShield.cpp
 * @brief Implementation of joystick shield reading functionality
 */
#include <Arduino.h>
#include "joystick_shield.h"
#include "pin_config.h"
#include "log.h"


// Constants
constexpr int kAnalogMinValue = 0;     ///< Minimum analog read value
constexpr int kAnalogMaxValue = 1023;  ///< Maximum analog read value
constexpr int kAnalogCenterValue = 512; ///< Approximate center value

constexpr int kButtonPressed = LOW;    ///< Button state when pressed (pull-up configuration)
constexpr int kButtonReleased = HIGH;  ///< Button state when released

constexpr int kDeadZoneThreshold = 100;        ///< Threshold from center to consider as movement
constexpr int kCalibrationButtonHoldTime = 3000;    ///< Time to hold button for calibration (ms)
constexpr unsigned long kCalibrationSamples = 5;  ///< Number of samples for calibration
constexpr unsigned long kCalibrationDelayMs = 10;   ///< Delay between calibration samples


/**
 * @brief Initializes all joystick shield pins
 */
void JoystickShieldSetup() {
    // Configure joystick button and control buttons with pull-up resistors
    pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
    pinMode(A_PIN, INPUT_PULLUP);
    pinMode(B_PIN, INPUT_PULLUP);
    pinMode(C_PIN, INPUT_PULLUP);
    pinMode(D_PIN, INPUT_PULLUP);
    pinMode(E_PIN, INPUT_PULLUP);
    pinMode(F_PIN, INPUT_PULLUP);
}

/**
 * @brief Reads all button states into the provided structure
 * @param states Pointer to ButtonStates structure to fill
 */
void ReadButtonStates(ButtonStates* states) {
    if (states == nullptr) return;

    states->joystick_button = (digitalRead(JOYSTICK_BUTTON_PIN) == kButtonPressed);
    states->button_a = (digitalRead(A_PIN) == kButtonPressed);
    states->button_b = (digitalRead(B_PIN) == kButtonPressed);
    states->button_c = (digitalRead(C_PIN) == kButtonPressed);
    states->button_d = (digitalRead(D_PIN) == kButtonPressed);
    states->button_e = (digitalRead(E_PIN) == kButtonPressed);
    states->button_f = (digitalRead(F_PIN) == kButtonPressed);
}

/**
 * @brief Performs auto-calibration of the joystick
 * @param cal_data Pointer to CalibrationData structure to fill
 */
void CalibrateJoystick(CalibrationData* cal_data) {
    if (cal_data == nullptr) return;

    // Initialize calibration values
    cal_data->x_min = kAnalogMinValue;
    cal_data->x_max = kAnalogMaxValue;
    cal_data->y_min = kAnalogMinValue;
    cal_data->y_max = kAnalogMaxValue;

    int32_t x_sum = 0;
    int32_t y_sum = 0;

    // Collect calibration samples
    for (unsigned int i = 0; i < kCalibrationSamples; i++) {
        int x_val = analogRead(JOYSTICK_X_PIN);
        int y_val = analogRead(JOYSTICK_Y_PIN);

        // Sum for center calculation
        x_sum += x_val;
        y_sum += y_val;

        delay(kCalibrationDelayMs);
    }

    // Calculate center values
    cal_data->x_center = x_sum / kCalibrationSamples;
    cal_data->y_center = y_sum / kCalibrationSamples;
    cal_data->calibrated = true;

    LOG_INFO("Calibration complete");
    PrintCalibrationData(cal_data);
}

/**
 * @brief Reads joystick data and applies calibration
 * @param data Pointer to JoystickData structure to fill
 * @param cal_data Pointer to CalibrationData for calibration
 */
void ReadJoystickData(JoystickData* data, const CalibrationData* cal_data) {
    if (data == nullptr) return;

    // Read raw values
    data->x_raw = analogRead(JOYSTICK_X_PIN);
    data->y_raw = analogRead(JOYSTICK_Y_PIN);

    // Apply calibration if available
    if (cal_data != nullptr && cal_data->calibrated) {
        // Normalize to -512 to +512 range around center
        if (data->x_raw > cal_data->x_center) {
            data->x_calibrated = map(data->x_raw, cal_data->x_center, cal_data->x_max, 0, 512);
        } else {
            data->x_calibrated = map(data->x_raw, cal_data->x_min, cal_data->x_center, -512, 0);
        }

        if (data->y_raw > cal_data->y_center) {
            data->y_calibrated = map(data->y_raw, cal_data->y_center, cal_data->y_max, 0, 512);
        } else {
            data->y_calibrated = map(data->y_raw, cal_data->y_min, cal_data->y_center, -512, 0);
        }

        // Clamp values to ensure they stay within bounds
        data->x_calibrated = constrain(data->x_calibrated, -512, 512);
        data->y_calibrated = constrain(data->y_calibrated, -512, 512);
    } else {
        // Use raw values if not calibrated
        data->x_calibrated = data->x_raw - kAnalogCenterValue;
        data->y_calibrated = data->y_raw - kAnalogCenterValue;
    }



    data->x_direction = GetJoystickDirection(data->x_calibrated + 512, kAnalogCenterValue, false);
    data->y_direction = GetJoystickDirection(data->y_calibrated + 512, kAnalogCenterValue, true);
}

/**
 * @brief Determines joystick direction from analog value with custom center
 * @param analog_value The raw analog reading (0-1023)
 * @param center_value The center value to use for comparison
 * @param is_vertical True if checking vertical axis, false for horizontal
 * @return JoystickDirection enum value
 */
JoystickDirection GetJoystickDirection(int analog_value, int center_value, bool is_vertical = false) {
    if (analog_value < (center_value - kDeadZoneThreshold)) {
        return is_vertical ? JoystickDirection::kDown : JoystickDirection::kLeft;
    } else if (analog_value > (center_value + kDeadZoneThreshold)) {
        return is_vertical ? JoystickDirection::kUp : JoystickDirection::kRight;
    } else {
        return JoystickDirection::kCenter;
    }
}

/**
 * @brief Converts direction enum to string for printing
 * @param direction The direction to convert
 * @return Constant character pointer to direction string
 */
const char* DirectionToString(JoystickDirection direction) {
    switch (direction) {
        case JoystickDirection::kLeft:   return "LEFT";
        case JoystickDirection::kRight:  return "RIGHT";
        case JoystickDirection::kUp:     return "UP";
        case JoystickDirection::kDown:   return "DOWN";
        case JoystickDirection::kCenter: return "CENTER";
        default:                         return "UNKNOWN";
    }
}

/**
 * @brief Prints active buttons to serial
 * @param states Pointer to ButtonStates structure
 */
void PrintActiveButtons(const ButtonStates* states) {
    if (states == nullptr) return;

    char buttons_str[10] = "";
    if (states->button_a) strncat(buttons_str, "A", sizeof(buttons_str) - strlen(buttons_str) - 1);
    if (states->button_b) strncat(buttons_str, "B", sizeof(buttons_str) - strlen(buttons_str) - 1);
    if (states->button_c) strncat(buttons_str, "C", sizeof(buttons_str) - strlen(buttons_str) - 1);
    if (states->button_d) strncat(buttons_str, "D", sizeof(buttons_str) - strlen(buttons_str) - 1);
    if (states->button_e) strncat(buttons_str, "E", sizeof(buttons_str) - strlen(buttons_str) - 1);
    if (states->button_f) strncat(buttons_str, "F", sizeof(buttons_str) - strlen(buttons_str) - 1);
    if (states->joystick_button) strncat(buttons_str, "JOY", sizeof(buttons_str) - strlen(buttons_str) - 1);

    if (strlen(buttons_str) != 0) {
        LOG_DEBUG("Buttons: %s", buttons_str);
    }
}

/**
 * @brief Prints joystick data to serial
 * @param data Pointer to JoystickData structure
 */
void PrintJoystickData(const JoystickData* data) {
    if (data == nullptr) return;

    LOG_DEBUG("Joystick: X[raw:%d, cal:%d, dir:%s], Y[raw:%d, cal:%d, dir:%s]",
             data->x_raw, data->x_calibrated, DirectionToString(data->x_direction),
             data->y_raw, data->y_calibrated, DirectionToString(data->y_direction));
}

/**
 * @brief Prints calibration data to serial
 * @param cal_data Pointer to CalibrationData structure
 */
void PrintCalibrationData(const CalibrationData* cal_data) {
    if (cal_data == nullptr) return;
    LOG_INFO("Calibration Data: X[min:%d, max:%d, center:%d], Y[min:%d, max:%d, center:%d]",
             cal_data->x_min, cal_data->x_max, cal_data->x_center,
             cal_data->y_min, cal_data->y_max, cal_data->y_center);
}
