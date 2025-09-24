#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>

enum class LogSource {
    UsbSerial,
    Serial1,
    UsbAndSerial1,
    NumberOfSerials
};

inline void initLog() {
    // Log.begin(LOG_LEVEL_VERBOSE, &Serial);
    Log.begin(LOG_LEVEL_TRACE, &Serial);
}

inline bool changeLogLevel(const size_t &log_level) {
    if (log_level <= LOG_LEVEL_VERBOSE) {
        Log.setLevel(log_level);
        return true;
    }
    return false;
}

inline void chengeLogSource(const LogSource &log_source) {
    switch (log_source) {
        case LogSource::UsbSerial:
            Log.begin(LOG_LEVEL_NOTICE, &Serial);
            break;
        // case LogSource::Serial1:
        //     Log.begin(LOG_LEVEL_NOTICE, &Serial1);
        //     break;
        // TODO: Implement this function
        // case LogSource::UsbAndSerial1:
        //     break;
        default:
            Log.begin(LOG_LEVEL_NOTICE, &Serial);
            break;
    }
}

inline unsigned long log_number = {};

// logCount [timestamp[ms]] filename:line_number - message
#define LOG_PREFIX "%l [%l] %s:%d- "
#define LOG_MS millis()
#ifndef __FILENAME__
#define __FILENAME__ (                                      \
    __builtin_strrchr(__FILE__, '/')                         \
      ? __builtin_strrchr(__FILE__, '/') + 1                 \
      : (                                                    \
          __builtin_strrchr(__FILE__, '\\')                  \
            ? __builtin_strrchr(__FILE__, '\\') + 1          \
            : __FILE__                                       \
        )                                                    \
)
#endif

// Arduino RAM optimization macros
#if defined(__AVR_ATmega328P__)
#include <Arduino.h>
#define RAM_OPT(ARG) F(ARG)
#else
#define RAM_OPT(ARG) ARG
#endif

#ifdef ENABLE_LOGGING
#define LOG_ERROR(MSG, ...) Log.errorln(RAM_OPT(LOG_PREFIX MSG), log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_WARNING(MSG, ...) Log.warningln(RAM_OPT(LOG_PREFIX MSG), log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_NOTICE(MSG, ...) Log.noticeln(RAM_OPT(LOG_PREFIX MSG), log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(MSG, ...) Log.infoln(RAM_OPT(LOG_PREFIX MSG), log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(MSG, ...) Log.traceln(RAM_OPT(LOG_PREFIX MSG), log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_VERBOSE(MSG, ...) Log.verboseln(RAM_OPT(LOG_PREFIX MSG), log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__)
#define LOG_FATAL(MSG, ...) Log.fatalln(RAM_OPT(LOG_PREFIX MSG), log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__); \
    while(1) { \
        delay(1000); \
    } // Infinite loop to halt execution after a fatal error
#else
#define LOG_FATAL(MSG, ...) Log.fatalln(RAM_OPT(LOG_PREFIX MSG), log_number++, LOG_MS, __FILENAME__, __LINE__, ##__VA_ARGS__); \
    while(1) { \
        delay(1000); \
    } // Infinite loop to halt execution after a fatal error
#define LOG_ERROR(MSG, ...)
#define LOG_WARNING(MSG, ...)
#define LOG_NOTICE(MSG, ...)
#define LOG_INFO(MSG, ...)
#define LOG_DEBUG(MSG, ...)
#define LOG_VERBOSE(MSG, ...)
#endif
