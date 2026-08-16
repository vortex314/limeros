#ifndef TIMER_H
#define TIMER_H
#include "actor_framework.h"

class Timer
{
private:
    uint64_t _expires_at;
    uint32_t _interval_ms;
    int _id;
    ActorId _owner;
    bool _running;
    bool _repeat;

public:
    Timer(ActorId owner, int id) : _expires_at(0), _interval_ms(0), _id(id), _owner(owner), _running(false), _repeat(false) {}
    Timer() : _expires_at(0), _interval_ms(0), _id(0), _owner(0), _running(false), _repeat(false) {}
    void setOwner(ActorId owner) { _owner = owner; }
    void start(uint32_t interval_ms, bool repeat = false);
    void stop();
    bool is_running() const;
    bool is_repeating() const { return _repeat; }
    bool is_expired() const;
    void update();
    int id() const { return _id; }
};

#endif // TIMER_H