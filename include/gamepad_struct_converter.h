#ifndef GAMEPAD_STRUCT_CONVERTER_H
#define GAMEPAD_STRUCT_CONVERTER_H

#include <string.h>
#include <stdio.h>

#include "log.h"
#include "joystick_shield_struct.h"
#include "Bluepad32_data_struct.h"

inline void convertGamepadDataToBP32(BP32Data::PackedControllerData &data, const PadData& pad_data) {
    // clear data structure
    memset(&data, 0, sizeof(BP32Data::PackedControllerData));
    // Map joystick data to BP32 data structure
    data.id = 0;  // Assuming single controller with ID 0
    data.dpad = 0; // D-pad not used
    data.axis_x = pad_data.joystick.x_calibrated;
    data.axis_y = pad_data.joystick.y_calibrated;

    data.buttons =
        (pad_data.buttons.button_a        ? BP32Data::ControllerButtonConst::kButtonA     : 0) |
        (pad_data.buttons.button_b        ? BP32Data::ControllerButtonConst::kButtonB     : 0) |
        (pad_data.buttons.button_c        ? BP32Data::ControllerButtonConst::kButtonX     : 0) | // Map C to X
        (pad_data.buttons.button_d        ? BP32Data::ControllerButtonConst::kButtonY     : 0) | // Map D to Y
        (pad_data.buttons.joystick_button ? BP32Data::ControllerButtonConst::kButtonThumbL: 0);  // Map joystick button to Left Thumb

    data.misc_buttons =
        (pad_data.buttons.button_e ? BP32Data::ControllerMiscConst::kButtonSelect : 0) |
        (pad_data.buttons.button_f ? BP32Data::ControllerMiscConst::kButtonStart  : 0);
}
inline void dump_bluepad_driver_data(const BP32Data::PackedControllerData & data) {
    if (data.id != -1) {
        char buf[256];
        snprintf(buf, sizeof(buf) - 1,
            "dpad: 0x%02x, buttons: 0x%04x, "
            "axis L: %4d, %4d, axis R: %4d, %4d, "
            "brake: %4d, throttle: %4d, misc: 0x%02x, "
            "gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d, ",
            static_cast<int>(data.dpad),          // DPAD
            (int)data.buttons,       // bitmask of pressed buttons
            (int)data.axis_x,       // (-511 - 512) left X Axis
            (int)data.axis_y,       // (-511 - 512) left Y axis
            (int)data.axis_rx,        // (-511 - 512) right X axis
            (int)data.axis_ry,        // (-511 - 512) right Y axis
            (int)data.brake,         // (0 - 1023): brake button
            (int)data.throttle,      // (0 - 1023): throttle (AKA gas) button
            static_cast<int>(data.misc_buttons),  // bitmak of pressed "misc" buttons
            data.gyro[0],       // Gyro X
            data.gyro[1],       // Gyro Y
            data.gyro[2],       // Gyro Z
            data.accel[0],      // Accelerometer X
            data.accel[1],      // Accelerometer Y
            data.accel[2]);     // Accelerometer Z
        LOG_DEBUG("%s", buf);
    }
}

#endif // GAMEPAD_STRUCT_CONVERTER_H