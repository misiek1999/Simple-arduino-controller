#include <Arduino.h>
#include "config.h"
#include "joystick_shield.h"
#include "log.h"
#include "pin_config.h"
#include "nrf24_driver.h"
#include "gamepad_struct_converter.h"


// Global data structures
JoystickData joystick_data;
ButtonStates button_states;
CalibrationData calibration_data;

//use single static instance of nrf24l01 driver
inline RF24Driver::NRF24Controller& getNRF24ControllerInstance() {
    static RF24Driver::NRF24Controller nrf24_controller(
        NRF24L01_CE_PIN,
        NRF24L01_CSN_PIN
    );
    return nrf24_controller;
}

void setup() {
    // Initialize serial communication
    Serial.begin(kSerialBaudRate);

    // Initialize logging
    initLog();

    // Initialize joystick shield
    JoystickShieldSetup();

    // Perform initial calibration
    CalibrateJoystick(&calibration_data);

    // Initialize NRF24L01 driver
    auto& nrf24 = getNRF24ControllerInstance();
    if (!nrf24.init()) {
        LOG_ERROR("Failed to initialize NRF24L01 driver");
    } else {
        LOG_INFO("NRF24L01 driver initialized successfully");
    }
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

    PadData pad_data{
        .joystick = joystick_data,
        .buttons = button_states
    };
    // Prepare controller data
    BP32Data::PackedControllerData controller_data{};
    convertGamepadDataToBP32(controller_data, pad_data);

    // Send controller data via NRF24L01
    auto& nrf24 = getNRF24ControllerInstance();
    if (nrf24.checkDriverIsInitialized()) {
        if (!nrf24.sendGamepadData(controller_data)) {
            LOG_DEBUG("Failed to send gamepad data");
        }
    } else {
        LOG_WARNING("NRF24L01 driver is not initialized");
    }

    dump_bluepad_driver_data(controller_data);
    delayMicroseconds(kMainLoopCycleTimeUs);
}
