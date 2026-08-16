#include "timer.h"


bool Timer::is_expired() const
{
    if (_expires_at > 0 && millis() >= _expires_at)
    {
        return true;
    }
    return false;
}

bool Timer::is_running() const
{
    return _running;
}
void Timer::start(uint32_t interval_ms, bool repeat)
{
    _interval_ms = interval_ms;
    _expires_at = millis() + interval_ms;
    _repeat = repeat;
    _running = true;
}

void Timer::stop()
{
    _running = false;
}

void Timer::update()
{
    if (!_running)
        return;

    if ( millis() >= _expires_at) {
        if (_repeat)
        {
            _expires_at += _interval_ms;
        }
        else
        {
            _running = false;
        }

        actor_system.send(_owner, _owner, new TimerMsg{_id});
    }
}
