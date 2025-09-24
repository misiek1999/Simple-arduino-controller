#ifndef CONFIG_H
#define CONFIG_H

// General configuration
constexpr auto kMainLoopCycleTimeUs = 10000UL;      // Constant time between next main loop iteration in us (10 ms = 100 Hz)
constexpr auto kSerialBaudRate = 250000;            // Serial communication baud rate

#endif // CONFIG_H
