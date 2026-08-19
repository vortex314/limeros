#ifndef TIMER_H
#define TIMER_H
#include "actor_framework.h"
#include "messages.h"

class Timer
{
private:
                uint64_t _expires_at;
                uint32_t _interval_ms;
    int _id;
    std::shared_ptr<ActorMessage> _timer_msg;
    bool _running;
    bool _repeat;

public:
    Timer(int id) : _expires_at(0), _interval_ms(0), _id(id), _timer_msg(nullptr), _running(false), _repeat(false) {
        _timer_msg = std::make_shared<ActorMessage>();
        _timer_msg->src = 0;
        _timer_msg->dst = 0;
        _timer_msg->set_payload(new TimerMsg(_id));
    }
   
    void setOwner(ActorId actor_id) { _timer_msg->src = actor_id; _timer_msg->dst = actor_id; }
    void start(uint32_t interval_ms, bool repeat = false);
    void stop();
    bool is_running() const;
    bool is_repeating() const { return _repeat; }
    bool is_expired() const;
    void update();
    int id() const { return _id; }
};

#endif // TIMER_H