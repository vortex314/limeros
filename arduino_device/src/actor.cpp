#include "app_compat.h"

// constexpr for FNV hashing of strings at compile time
constexpr uint32_t fnv1a_32(const char *str, size_t len, uint32_t hash = 0x811c9dc5)
{
    return (len == 0) ? hash : fnv1a_32(str + 1, len - 1, (hash ^ static_cast<uint32_t>(*str)) * 0x01000193);
}

#define FNV1A32(str) (fnv1a_32(str, sizeof(str) - 1))

#define DEFINE_MSG(Name, ...)                              \
    struct Name                                            \
    {                                                      \
        static constexpr uint32_t MSG_ID = FNV1A32(#Name); \
        __VA_ARGS__                                        \
    };

DEFINE_MSG(TimerMsg, int timer_id;)
DEFINE_MSG(LogMsg, char text[24];)
DEFINE_MSG(SensorMsg, float value; uint8_t id;)

enum class MsgType : uint32_t
{
    None = 0,
    Timer = FNV1A32("TimerMsg"),
    Log = FNV1A32("LogMsg"),
    Sensor = FNV1A32("SensorMsg"),
    // add more...
};

// struct TimerMsg  { int timer_id; };
// struct LogMsg    { char text[24]; };
// struct SensorMsg { float value; uint8_t id; };

struct ActorMessage
{
    uint8_t sender = 0;
    uint8_t receiver = 0;
    MsgType type = MsgType::None;

    union
    {
        TimerMsg timer;
        LogMsg log;
        SensorMsg sensor;
        // add every message type here
    };

    // ---------- helpers ----------
    template <typename T>
    bool is() const
    {
        return type == static_cast<MsgType>(T::MSG_ID);
    }

    // Safe typed access
    template <typename T>
    const T *get() const
    {
        if (!is<T>())
            return nullptr;
        return reinterpret_cast<const T *>(&timer); // all members start at the same address
    }

    // Nice handle syntax (no std::function)
    template <typename T, typename Func>
    bool handle(Func &&func) const
    {
        if (const T *p = get<T>())
        {
            func(*p); // or func(p) if you prefer pointer
            return true;
        }
        return false;
    }
};

class Actor
{
public:
    uint8_t id = 0;
    virtual void setup() {}                            // called once after registration
    virtual void receive(const ActorMessage &msg) = 0; // mandatory
    virtual void update() {}                           // optional (timers, polling…)
    virtual ~Actor() {}
};

class SerialActor : public Actor
{
public:
    void setup() override
    {
        Serial.begin(115200);
    }
    void receive(const ActorMessage &msg) override
    {
        msg.handle<TimerMsg>([](const TimerMsg &t)
                             {
            Serial.print("TimerMsg received: ");
            Serial.println(t.timer_id); });
        msg.handle<LogMsg>([](const LogMsg &l)
                           {
            Serial.print("LogMsg received: ");
            Serial.println(l.text); });
        msg.handle<SensorMsg>([](const SensorMsg &s)
                              {
            Serial.print("SensorMsg received: ");
            Serial.print(s.value);
            Serial.print(", id: ");
            Serial.println(s.id); });
    }
    void update() override
    {
        // nothing to do
    };
};

void fn()
{
    ActorMessage msg;
    msg.sender = 1;
    msg.receiver = 2;
    msg.type = MsgType::Timer;
    msg.timer.timer_id = 42;

    // Style 1
    msg.handle<TimerMsg>([](const TimerMsg &t)
                         { Serial.println(t.timer_id); });

    // Style 2
    if (auto *t = msg.get<TimerMsg>())
    {
        Serial.println(t->timer_id);
    }
}