#include "actor_framework.h"

// -------------------- Logger --------------------
class LoggerActor : public Actor
{
public:
    void receive(const ActorMessage& msg) override
    {
        msg.handle<LogMsg>([](const LogMsg& l) {
            Serial.print(F("[LOG] "));
            Serial.println(l.text);
        });

        msg.handle<TimerMsg>([](const TimerMsg& t) {
            Serial.print(F("[TIMER] id="));
            Serial.println(t.timer_id);
        });
    }
};

// -------------------- Blinker --------------------
class BlinkActor : public Actor
{
    uint8_t  pin;
    bool     state = false;
    uint16_t interval = 500;
    uint32_t lastToggle = 0;

public:
    explicit BlinkActor(uint8_t ledPin) : pin(ledPin) {}

    void setup() override
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    void receive(const ActorMessage& msg) override
    {
        msg.handle<ToggleMsg>([this](const ToggleMsg&) {
            state = !state;
            digitalWrite(pin, state ? HIGH : LOW);
        });

        msg.handle<SetIntervalMsg>([this](const SetIntervalMsg& m) {
            interval = m.ms;
        });
    }

    void update() override
    {
        if (millis() - lastToggle >= interval)
        {
            lastToggle = millis();
            actor_system.sendToggle(id, id);   // toggle ourselves
        }
    }
};

// -------------------- Instances --------------------
/*LoggerActor logger;
BlinkActor  blinker(RED_LED);

void setup()
{
    Serial.begin(115200);
    delay(300);

    System.registerActor(&logger);   // id = 1
    System.registerActor(&blinker);  // id = 2

    // Change speed after start
    System.sendSetInterval(2, 300);
}

void loop()
{
    System.run();
}*/