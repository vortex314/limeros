
#include "led_actor.h"

LedActor::LedActor(uint8_t ledPin) : _pin(ledPin), _timer(1) {}

void LedActor::setup()
{
    // Bind the timer to THIS actor. The owner captured in the constructor is
    // always 0 because Actor::id is not assigned until registerActor() runs
    // (which is after construction but before setup()). Without this, every
    // LedActor's timer message would be routed to actor 0 (the red LED).
    _timer.setOwner(id);

    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    _timer.start(100, true); // Start the timer with a 100 msec
}

void LedActor::receive(std::shared_ptr<ActorMessage> msg)
{

    msg->handle<LedPulse>([this](const LedPulse &m)
                          {
                                   _timer.start(m.duration_ms, false);
                                   digitalWrite(_pin, HIGH);
                                   _state = LedState::PULSING; });
    msg->handle<LedBlink>([this](const LedBlink &m)
                          {
                                   _timer.start(m.interval_ms, true);
                                   _state = LedState::BLINKING; });
    msg->handle<LedOn>([this](const LedOn &m)
                       {
                                   _timer.stop();
                                   digitalWrite(_pin, m.on ? HIGH : LOW);
                                   _state = m.on ? LedState::ON : LedState::OFF; });

    msg->handle<TimerMsg>([this](const TimerMsg &m)
                          {
                             if (m.timer_id == _timer.id())
                             {
                                 if (_state == LedState::PULSING)
                                 {
                                     digitalWrite(_pin, LOW);
                                     _state = LedState::OFF;

                                 } else if (_state == LedState::BLINKING)
                                 {
                                     _ledOn = !_ledOn;
                                     digitalWrite(_pin, _ledOn ? HIGH : LOW);
                                 }
                             } });
}
void LedActor::update()
{
    _timer.update();
}
