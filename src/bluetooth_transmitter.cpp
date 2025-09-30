#include "bluetooth_transmitter.h"
#include "log.h"

BluetoothTransmitter::BluetoothTransmitter()
    : BluetoothTransmitter(&Serial)
{
}

BluetoothTransmitter::BluetoothTransmitter(HardwareSerial *serial_ble)
    : serial_ble_(serial_ble),
      last_command_(""),
      last_command_time_(0),
      is_initialized_(false)
{
    LOG_INFO("BluetoothTransmitter initialized with custom HardwareSerial");
}

bool BluetoothTransmitter::Initialize()
{
    serial_ble_->end(); // Ensure any previous connection is closed
    serial_ble_->begin(kBaudRate);
    // TODO: Toggle EN pin when AT commands are send
    serial_ble_->print("AT+BAUD3");
    serial_ble_->end(); // Ensure any previous connection is closed
    serial_ble_->begin(38400);

    is_initialized_ = true;
    return is_initialized_;
}

bool BluetoothTransmitter::SendSpeedCommand(int16_t x_value, int16_t y_value)
{
    if (!ValidatePadValues(x_value, y_value))
    {
        return false;
    }

    String command = BuildPadCommand(CommandType::kSpeed, x_value, y_value);
    return SendCommand(command);
}

bool BluetoothTransmitter::SendRotationCommand(int16_t x_value, int16_t y_value)
{
    if (!ValidatePadValues(x_value, y_value))
    {
        return false;
    }

    String command = BuildPadCommand(CommandType::kRotate, x_value, y_value);
    return SendCommand(command);
}

bool BluetoothTransmitter::SendAngleOffsetIncrease()
{
    String command = String(static_cast<char>(CommandType::kAngleOffsetIncrease));
    command += kMessageDelimiter;
    return SendCommand(command);
}

bool BluetoothTransmitter::SendAngleOffsetDecrease()
{
    String command = String(static_cast<char>(CommandType::kAngleOffsetDecrease));
    command += kMessageDelimiter;
    return SendCommand(command);
}

bool BluetoothTransmitter::SendPidSetting1(const String &data)
{
    String command = String(static_cast<char>(CommandType::kPidSetting1));
    command += data;
    command += kMessageDelimiter;
    return SendCommand(command);
}

bool BluetoothTransmitter::SendPidSetting2(const String &data)
{
    String command = String(static_cast<char>(CommandType::kPidSetting2));
    command += data;
    command += kMessageDelimiter;
    return SendCommand(command);
}

bool BluetoothTransmitter::SendPidSetting3(const String &data)
{
    String command = String(static_cast<char>(CommandType::kPidSetting3));
    command += data;
    command += kMessageDelimiter;
    return SendCommand(command);
}

bool BluetoothTransmitter::SendPidSetting4(const String &data)
{
    String command = String(static_cast<char>(CommandType::kPidSetting4));
    command += data;
    command += kMessageDelimiter;
    return SendCommand(command);
}

bool BluetoothTransmitter::SendRawCommand(const String &command)
{
    if (command.length() > kMaxCommandLength)
    {
        return false;
    }

    String formatted_command = command;
    // Add delimiter if not present
    if (formatted_command[formatted_command.length() - 1] != kMessageDelimiter)
    {
        formatted_command += kMessageDelimiter;
    }

    return SendCommand(formatted_command);
}

bool BluetoothTransmitter::IsConnected() const
{
    return is_initialized_;
    // Note: For more robust connection checking, you might want to
    // implement a heartbeat mechanism or check for responses
}

unsigned long BluetoothTransmitter::GetTimeSinceLastCommand() const
{
    if (last_command_time_ == 0)
    {
        return 0;
    }
    return millis() - last_command_time_;
}

String BluetoothTransmitter::BuildPadCommand(CommandType type,
                                             int16_t x_value,
                                             int16_t y_value)
{
    String command = String(static_cast<char>(type));
    command += "X";
    command += String(x_value);
    command += "Y";
    command += String(y_value);
    command += kMessageDelimiter;

    return command;
}

bool BluetoothTransmitter::SendCommand(const String &command)
{
    if (!is_initialized_)
    {
        LOG_ERROR("Error: Controller not initialized");
        return false;
    }

    if (command.length() == 0 || command.length() > kMaxCommandLength)
    {
        LOG_WARNING("Error: Invalid command length");
        return false;
    }

    // Send the command
    serial_ble_->print(command);

    // Update tracking variables
    last_command_ = command;
    last_command_time_ = millis();

    // Debug output (optional)
    LOG_DEBUG("Sent: %s", command.c_str());

    return true;
}

bool BluetoothTransmitter::ValidatePadValues(int16_t x_value, int16_t y_value) const
{
    if (x_value < kMinPadValue || x_value > kMaxPadValue)
    {
        LOG_WARNING("X value out of range: %d", x_value);
        return false;
    }

    if (y_value < kMinPadValue || y_value > kMaxPadValue)
    {
        LOG_WARNING("Y value out of range: %d", y_value);
        return false;
    }

    return true;
}
