#ifndef LED_ACTOR_H
#define LED_ACTOR_H
#include <actor_framework.h>
#include "timer.h"
#include "messages.h"

class LedActor : public Actor
{
private:
    uint8_t _pin;
    Timer _timer;
    enum class LedState
    {
        OFF,
        ON,
        BLINKING,
        PULSING,
    } _state = LedState::BLINKING;
    bool _ledOn = false;


public:
    explicit LedActor(uint8_t ledPin);
    void setup() override;
    void update() override;
    void receive(std::shared_ptr<ActorMessage> msg) override;
};

#endif // LED_ACTOR_H