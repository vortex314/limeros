#pragma once

#include <Arduino.h>
#include <string.h>   // for strncpy

// ============================================================
// Compile-time FNV-1a hash
// ============================================================
constexpr uint32_t fnv1a_32(const char* str, size_t len, uint32_t hash = 0x811c9dc5)
{
    return (len == 0) ? hash
                      : fnv1a_32(str + 1, len - 1,
                                 (hash ^ static_cast<uint32_t>(*str)) * 0x01000193);
}

#define FNV1A32(str) (fnv1a_32(str, sizeof(str) - 1))

// ============================================================
// Message definition helper
// ============================================================
#define DEFINE_MSG(Name, ...)                              \
    struct Name                                            \
    {                                                      \
        static constexpr uint32_t MSG_ID = FNV1A32(#Name); \
        __VA_ARGS__                                        \
    };

// ============================================================
// Built-in message types (add your own below)
// ============================================================
DEFINE_MSG(TimerMsg,   int timer_id;)
DEFINE_MSG(LogMsg,     char text[24];)
DEFINE_MSG(SensorMsg,  float value; uint8_t id;)
DEFINE_MSG(ToggleMsg,  ; )                          // empty payload
DEFINE_MSG(SetIntervalMsg, uint16_t ms;)

// ============================================================
// ActorMessage
// ============================================================
struct ActorMessage
{
    uint8_t  sender   = 0;
    uint8_t  receiver = 0;
    uint32_t type     = 0;

    union
    {
        TimerMsg       timer;
        LogMsg         log;
        SensorMsg      sensor;
        ToggleMsg      toggle;
        SetIntervalMsg setInterval;
        // >>> add new message types here <<<
    };

    // ---- type-safe helpers ----
    template <typename T>
    bool is() const { return type == T::MSG_ID; }

    template <typename T>
    const T* get() const
    {
        return is<T>() ? reinterpret_cast<const T*>(&timer) : nullptr;
    }

    template <typename T>
    T* get()
    {
        return is<T>() ? reinterpret_cast<T*>(&timer) : nullptr;
    }

    // handle() – returns true if the message was of type T
    template <typename T, typename Func>
    bool handle(Func&& func) const
    {
        if (const T* p = get<T>())
        {
            func(*p);
            return true;
        }
        return false;
    }

    template <typename T, typename Func>
    bool handle(Func&& func)
    {
        if (T* p = get<T>())
        {
            func(*p);
            return true;
        }
        return false;
    }
};

// ============================================================
// Factory helpers (recommended way to create messages)
// ============================================================
inline ActorMessage makeTimerMsg(uint8_t from, uint8_t to, int timer_id)
{
    ActorMessage m;
    m.sender   = from;
    m.receiver = to;
    m.type     = TimerMsg::MSG_ID;
    m.timer.timer_id = timer_id;
    return m;
}

inline ActorMessage makeLogMsg(uint8_t from, uint8_t to, const char* text)
{
    ActorMessage m;
    m.sender   = from;
    m.receiver = to;
    m.type     = LogMsg::MSG_ID;
    strncpy(m.log.text, text, sizeof(m.log.text) - 1);
    m.log.text[sizeof(m.log.text) - 1] = '\0';
    return m;
}

inline ActorMessage makeSensorMsg(uint8_t from, uint8_t to, float value, uint8_t id)
{
    ActorMessage m;
    m.sender       = from;
    m.receiver     = to;
    m.type         = SensorMsg::MSG_ID;
    m.sensor.value = value;
    m.sensor.id    = id;
    return m;
}

inline ActorMessage makeToggleMsg(uint8_t from, uint8_t to)
{
    ActorMessage m;
    m.sender   = from;
    m.receiver = to;
    m.type     = ToggleMsg::MSG_ID;
    return m;
}

inline ActorMessage makeSetIntervalMsg(uint8_t from, uint8_t to, uint16_t ms)
{
    ActorMessage m;
    m.sender           = from;
    m.receiver         = to;
    m.type             = SetIntervalMsg::MSG_ID;
    m.setInterval.ms   = ms;
    return m;
}

// ============================================================
// Actor base class
// ============================================================
class Actor
{
public:
    uint8_t id = 0;

    virtual void setup() {}
    virtual void receive(const ActorMessage& msg) = 0;
    virtual void update() {}

    virtual ~Actor() {}
};

// ============================================================
// ActorSystem
// ============================================================
#ifndef MAX_ACTORS
#define MAX_ACTORS 8
#endif

#ifndef MAX_MESSAGES
#define MAX_MESSAGES 16
#endif

class ActorSystem
{
public:
    uint8_t registerActor(Actor* actor)
    {
        if (!actor || numActors >= MAX_ACTORS)
            return 0;

        actor->id = ++numActors;
        actors[actor->id] = actor;
        actor->setup();
        return actor->id;
    }

    bool send(const ActorMessage& msg)
    {
        if (queueCount >= MAX_MESSAGES)
            return false;
        if (msg.receiver == 0 || msg.receiver > numActors)
            return false;

        queue[queueTail] = msg;
        queueTail = (queueTail + 1) % MAX_MESSAGES;
        ++queueCount;
        return true;
    }

    // convenience wrappers
    bool sendTimer   (uint8_t to, int id,          uint8_t from = 0) { return send(makeTimerMsg(from, to, id)); }
    bool sendLog     (uint8_t to, const char* txt, uint8_t from = 0) { return send(makeLogMsg(from, to, txt)); }
    bool sendToggle  (uint8_t to,                  uint8_t from = 0) { return send(makeToggleMsg(from, to)); }
    bool sendSetInterval(uint8_t to, uint16_t ms,  uint8_t from = 0) { return send(makeSetIntervalMsg(from, to, ms)); }

    void processOne()
    {
        if (queueCount == 0)
            return;

        ActorMessage msg = queue[queueHead];
        queueHead = (queueHead + 1) % MAX_MESSAGES;
        --queueCount;

        if (msg.receiver >= 1 && msg.receiver <= numActors && actors[msg.receiver])
            actors[msg.receiver]->receive(msg);
    }

    void processAll()
    {
        while (queueCount > 0)
            processOne();
    }

    void updateAll()
    {
        for (uint8_t i = 1; i <= numActors; ++i)
            if (actors[i])
                actors[i]->update();
    }

    void run()
    {
        processAll();
        updateAll();
    }

    uint8_t actorCount() const { return numActors; }
    uint8_t queueSize()  const { return queueCount; }

private:
    Actor*        actors[MAX_ACTORS + 1] = {};
    ActorMessage  queue[MAX_MESSAGES];
    uint8_t       queueHead  = 0;
    uint8_t       queueTail  = 0;
    uint8_t       queueCount = 0;
    uint8_t       numActors  = 0;
};

// Global instance
extern ActorSystem System;