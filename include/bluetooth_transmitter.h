#ifndef BLUETOOTH_TRANSMITTER_H_
#define BLUETOOTH_TRANSMITTER_H_

#include <Arduino.h>

// Command types for communication protocol
enum class CommandType : char
{
    kSpeed = 'S',
    kRotate = 'R',
    kAngleOffsetIncrease = 'A',
    kAngleOffsetDecrease = 'B',
    kPidSetting1 = 'C',
    kPidSetting2 = 'D',
    kPidSetting3 = '3',
    kPidSetting4 = '4'
};

// Controller class for sending commands via Bluetooth serial
class BluetoothTransmitter
{
public:
    // Default constructor using predefined TX and RX pins
    BluetoothTransmitter();
    // Constructor with TX and RX pins for Custom Serial
    BluetoothTransmitter(HardwareSerial* serial_ble);

    // Destructor
    ~BluetoothTransmitter() = default;

    // Initialize the Bluetooth connection
    bool Initialize();

    // Send speed control command (first pad)
    // @param x_value: X-axis value (-127 to 127)
    // @param y_value: Y-axis value (-127 to 127)
    bool SendSpeedCommand(int16_t x_value, int16_t y_value);

    // Send rotation control command (second pad)
    // @param x_value: X-axis value (-127 to 127)
    // @param y_value: Y-axis value (-127 to 127)
    bool SendRotationCommand(int16_t x_value, int16_t y_value);

    // Send angle offset increase command
    bool SendAngleOffsetIncrease();

    // Send angle offset decrease command
    bool SendAngleOffsetDecrease();

    // Send PID setting commands
    bool SendPidSetting1(const String &data = "");
    bool SendPidSetting2(const String &data = "");
    bool SendPidSetting3(const String &data = "");
    bool SendPidSetting4(const String &data = "");

    // Send raw command string
    bool SendRawCommand(const String &command);

    // Check if connection is available
    bool IsConnected() const;

    // Get last command sent
    String GetLastCommand() const { return last_command_; }

    // Get time since last command was sent (in milliseconds)
    unsigned long GetTimeSinceLastCommand() const;

private:
    // Constants
    static constexpr uint32_t kBaudRate = 9600;
    static constexpr char kMessageDelimiter = '*';
    static constexpr int16_t kMinPadValue = -127;
    static constexpr int16_t kMaxPadValue = 127;
    static constexpr size_t kMaxCommandLength = 32;

    // Helper method to build pad command string
    String BuildPadCommand(CommandType type, int16_t x_value, int16_t y_value);

    // Helper method to send command via serial
    bool SendCommand(const String &command);

    // Validate pad values are within range
    bool ValidatePadValues(int16_t x_value, int16_t y_value) const;

    // Member variables
    HardwareSerial* serial_ble_;
    String last_command_;
    unsigned long last_command_time_;
    bool is_initialized_;
};

#endif // BLUETOOTH_TRANSMITTER_H_
