#ifndef MESSAGES_H
#define MESSAGES_H
#pragma once

#include <cstdint>
#include <cstring>
#include <option.h>
#include <vector>
#include <string>

#define DEFINE_MSG(Name, ...)                              \
    struct Name                                            \
    {                                                      \
        static constexpr uint32_t MSG_ID = FNV1A32(#Name); \
        __VA_ARGS__                                        \
    };

// constexpr for FNV hashing of strings at compile time
constexpr uint32_t fnv1a_32(const char *str, size_t len, uint32_t hash = 0x811c9dc5)
{
    return (len == 0) ? hash : fnv1a_32(str + 1, len - 1, (hash ^ static_cast<uint32_t>(*str)) * 0x01000193);
}

#define FNV1A32(str) (fnv1a_32(str, sizeof(str) - 1))

// ============================================================
// Built-in message types (add your own below)
// ============================================================
DEFINE_MSG(TimerMsg, int timer_id;)
DEFINE_MSG(LogMsg, char text[24];)
DEFINE_MSG(SensorMsg, float value; uint8_t id;)
DEFINE_MSG(ToggleMsg, ;) // empty payload
DEFINE_MSG(SetIntervalMsg, uint16_t ms;)
DEFINE_MSG(SerialSendFrameMsg, uint8_t *data; size_t length;)
DEFINE_MSG(SerialReceiveFrameMsg, uint8_t *data; size_t length;)
DEFINE_MSG(LedPulse, uint32_t duration_ms;)
DEFINE_MSG(LedBlink, uint32_t interval_ms;)
DEFINE_MSG(EnvelopeMsg, uint32_t src; uint32_t msg_type; Option<uint32_t> dst; Option<uint32_t> sequence; Option<std::vector<uint8_t>> payload;)

typedef union
{
    TimerMsg timerMsg;
    LogMsg logMsg;
    SensorMsg sensorMsg;
    ToggleMsg toggleMsg;
    SetIntervalMsg setIntervalMsg;
    SerialSendFrameMsg serialSendFrameMsg;
    SerialReceiveFrameMsg serialReceiveFrameMsg;
    LedPulse ledPulse;
    LedBlink ledBlink;
} MessagePayload;

#endif // MESSAGES_H
