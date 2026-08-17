#ifndef MESSAGES_H
#define MESSAGES_H
#pragma once

#include <cstdint>
#include <cstring>
#include <option.h>
#include <vector>
#include <string>
#include <msg.h>
#include <actor_framework.h>


// ============================================================
// Built-in message types (add your own below)
// ============================================================

DEFINE_MSG(TimerMsg, int timer_id;TimerMsg(int id) : timer_id(id) {})
DEFINE_MSG(LogMsg, char text[24];)
DEFINE_MSG(SensorMsg, float value; uint8_t id;)
DEFINE_MSG(ToggleMsg, ;) // empty payload
DEFINE_MSG(SetIntervalMsg, uint16_t ms;)
DEFINE_MSG(SerialSendFrameMsg, uint8_t *data; size_t length;)
DEFINE_MSG(SerialReceiveFrameMsg, uint8_t *data; size_t length;)
DEFINE_MSG(LedPulse, uint32_t duration_ms;LedPulse(uint32_t duration) : duration_ms(duration) {})
DEFINE_MSG(LedBlink, uint32_t interval_ms;LedBlink(uint32_t interval) : interval_ms(interval) {})


#endif // MESSAGES_H
