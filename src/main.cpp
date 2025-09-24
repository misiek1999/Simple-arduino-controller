#include <Arduino.h>
#include "config.h"
#include "joystick_shield.h"
#include "log.h"

// Global data structures
JoystickData joystick_data;
ButtonStates button_states;
CalibrationData calibration_data;

void setup() {
    // Initialize serial communication
    Serial.begin(kSerialBaudRate);

    // Initialize logging
    initLog();

    // Initialize joystick shield
    JoystickShieldSetup();

    // Perform initial calibration
    CalibrateJoystick(&calibration_data);

    LOG_INFO("Setup complete");
}

void loop() {
    // Read joystick data with calibration
    ReadJoystickData(&joystick_data, &calibration_data);
    // Read button states
    ReadButtonStates(&button_states);

    // Print joystick and button data
    PrintJoystickData(&joystick_data);
    PrintActiveButtons(&button_states);

    delayMicroseconds(kMainLoopCycleTimeUs);
}
