#ifndef ACTOR_H
#define ACTOR_H
#include <freertos/FreeRTOS.h>
#include <freertos/mpu_wrappers.h>
#include <freertos/portmacro.h>
#include <freertos/queue.h>
#include <freertos/timers.h>
#include <stddef.h>
#include <vector>
#include <functional>
#include <esp_timer.h>
#include <log.h>
#include <util.h>
#include <serdes.h>
#include <option.h>
#include <result.h>
#include <memory>
#include <stdint.h>
#include <serdes.h>
#include <ArduinoJson.h>
#include <msgs.h>
#pragma once

uint64_t current_time();

class ActorRef
{
    std::string _actor_name;
    bool _local = false;

public:
    ActorRef() = delete;
    constexpr ActorRef(const char *name) : _actor_name(name) {};
    ActorRef(const ActorRef &other) : _actor_name(other._actor_name) {};
    bool operator==(ActorRef &other) { return _actor_name == other._actor_name; }; // matches same id
    void operator=(ActorRef other) { _actor_name = other._actor_name; }
    bool match_name(ActorRef &other) { return strcmp(_actor_name.c_str(), other._actor_name.c_str()) == 0; };
    inline const char *name() const { return _actor_name.c_str(); };
    bool is_local() const { return _local; }
    void set_local(bool local) { _local = local; }
    ~ActorRef()= default;
};

extern ActorRef NULL_ACTOR;

class ActorMessage
{
public:
    std::optional<ActorRef> src = std::nullopt;
    std::optional<ActorRef> dst = std::nullopt;
    const Msg *msg;
    ActorMessage(Msg *msg) : msg(msg) {}
    ActorMessage(ActorRef src, const Msg *msg) : src(src), msg(msg) {}
    ActorMessage(ActorRef src, ActorRef dst, const Msg *msg) : src(src), dst(dst), msg(msg) {}
    ~ActorMessage() { delete msg; }
};

class TimerMsg : public Msg
{
public:
    static uint32_t msg_id() { return FNV("TimerMsg"); }
    static const char *msg_name() { return "TimerMsg"; }
    int timer_id;
    TimerMsg(int id) : timer_id(id) {}
};


template <class T>
class Queue
{
private:
    QueueHandle_t queue;
    size_t _depth;

public:
    Queue(size_t depth) : _depth(depth)
    {
        queue = xQueueCreate(depth, sizeof(T));
        INFO("Channel created [%d][%d]", depth, sizeof(T));
    }

    int getDepth() const { return _depth; }
    int getSize() const { return uxQueueMessagesWaiting(queue); }

    bool send(T qt, TickType_t timeout = 100 / portTICK_PERIOD_MS)
    {
        return xQueueSend(queue, &qt, timeout) == pdTRUE ? true : false;
    }

    bool sendFromIsr(T qt, TickType_t timeout = 100 / portTICK_PERIOD_MS)
    {
        return xQueueSendFromISR(queue, &qt, nullptr) == pdTRUE;
    }

    bool receive(T *qt, TickType_t timeout = portMAX_DELAY)
    {
        if (xQueueReceive(queue, qt, timeout) == pdTRUE)
        {
            return true;
        };
        return false;
    }
    size_t size() { return uxQueueMessagesWaiting(queue); }
    ~Queue() { vQueueDelete(queue); }
    QueueHandle_t getQueue() { return queue; }
    size_t getQueueDepth() { return _depth; }
};

// Queue Set wrapper
class QueueSet
{
private:
    QueueSetHandle_t set;

public:
    QueueSet(size_t maxQueues) { set = xQueueCreateSet(maxQueues); }

    inline void addQueue(QueueHandle_t queue)
    {
        if (xQueueAddToSet(queue, set) != pdTRUE)
        {
            ERROR("Failed to add queue to set");
        }
    }

    inline QueueHandle_t wait(TickType_t timeout = portMAX_DELAY)
    {
        return xQueueSelectFromSet(set, timeout);
    }

    inline ~QueueSet() { vQueueDelete(set); }
};

struct Timer
{
    bool _auto_reload;
    uint64_t _expires_at;
    uint64_t _period;
    bool _active = false;

public:
    Timer(bool auto_reload, bool active, uint64_t period,
          uint64_t expires_at);
    static Timer Repetitive(uint64_t period);
    static Timer OneShot(uint64_t delay);
    void make_one_shot(uint64_t delay);
    void make_repetitive(uint64_t period);
    bool is_expired(uint64_t now) const &;
    void refresh(uint64_t now);
    void start();
    inline void stop() { _active = false; }
    void reset();
    void fire(uint64_t delay);
};

void refresh_expired_timers(std::vector<Timer> &timers);

class Timers
{
    std::vector<Timer> _timers;

public:
    Timers() {}
    uint64_t get_next_expires_at();
    uint64_t sleep_time();
    std::vector<int> get_expired_timers();
    void refresh_expired_timers();
    void refresh(int id);
    int create_one_shot(uint64_t delay);
    int create_repetitive(uint64_t period);
    int start();
    void fire(int id, uint64_t delay);
    void stop(int id);
};

class ThreadSupport
{
public:
    virtual void on_start() = 0;
    virtual void on_stop() = 0;
    virtual QueueHandle_t queue_handle() = 0;
    Option<QueueHandle_t> additional_queue() { return Option<QueueHandle_t>::None(); }
    virtual uint64_t sleep_time() = 0;
    virtual void handle_expired_timers() = 0;
    virtual const char *name() = 0;
    virtual ~ThreadSupport() = default;
};

class Actor;

class EventBus : public Queue<const ActorMessage *>
{
    std::vector<Actor *> _actors;
    std::vector<std::function<void(const ActorMessage &)>> _message_handlers;
    size_t _stack_size = 1024;
    TaskHandle_t _task_handle;

public:
    EventBus(size_t size);
    void push(const ActorMessage *env);
    void register_actor(Actor *);
    void register_handler(std::function<void(const ActorMessage &)> handler)
    {
        _message_handlers.push_back(handler);
    }
    void loop();
    void start();
    ActorRef find_actor(const char *name);
    const std::vector<Actor *> &actors() { return _actors; }
};

class Actor
{
private:
    ActorRef _self;
    Timers _timers;
    EventBus *_eventbus = nullptr;
    std::vector<const char *> _interests; // message type names this actor handles

public:
    Actor(const char *name) : _self(name) { _self.set_local(true); };
    ~Actor() { INFO("Destroying actor %s", name()); }

    virtual void on_start() { INFO("actor %s default started.", _self.name()); }
    virtual void on_stop() { INFO("actor %s default stopped.", _self.name()); }
    virtual void on_message(const ActorMessage &env)
    {
        WARN(" No message handler for actor %s ", _self.name());
    }

    /// Declare that this actor handles messages of the given type name.
    /// Call in the constructor. If no interests are registered, the actor
    /// receives all messages (backward-compatible default).
    void add_interest(const char *type_name) { _interests.push_back(type_name); }

    /// Returns true if this actor should receive messages of the given type.
    /// An empty interest list means "accept all".
    bool accepts(const char *type_name) const
    {
        if (_interests.empty())
            return true;
        for (auto &interest : _interests)
        {
            if (strcmp(interest, type_name) == 0)
                return true;
        }
        return false;
    }
    std::vector<int> get_expired_timers() { return _timers.get_expired_timers(); }
    Timers &timers() { return _timers; }
    uint64_t sleep_time() { return _timers.sleep_time(); }
    ActorRef ref() const { return _self; }
    const char *name() { return _self.name(); }
    EventBus *eventbus() const { return _eventbus; }

    void emit(const Msg *msg);
    void emit(const ActorMessage *env);
    void set_eventbus(EventBus *eventbus);

    void handle_expired_timers()
    {
        for (int id : _timers.get_expired_timers())
        {
            ActorMessage *env = new ActorMessage(ref(), new TimerMsg(id));
            on_message(*env);
            _timers.refresh(id);
        }
    };
    void loop() {};
    void stop() {}
    int timer_one_shot(uint64_t delay) { return _timers.create_one_shot(delay); }
    int timer_repetitive(uint64_t period) { return _timers.create_repetitive(period); }
    void timer_stop(int id) { _timers.stop(id); }
    void timer_start(int id, uint64_t delay) { _timers.refresh(id); }
    void refresh(int id) { _timers.refresh(id); }
    void timer_fire(int id, uint64_t delay) { _timers.fire(id, delay); }
};

/*

A thread is created to manage multiple actors. The thread will wait for the
actor with the lowest sleep time and handle the command or timer event.

NOTE: The `Thread` class is currently unused in production. `main.cpp` runs
a single-threaded `EventBus::loop()` in `app_main` which fans messages to all
actors sequentially. The `Thread` infrastructure (per-actor FreeRTOS queues
with `QueueSet` multiplexing) is available for actors that need isolation
(e.g., blocking I/O, OTA writes) but has not yet been adopted. Evaluate
whether to adopt it or remove it before adding new actors.

*/

typedef enum Cpu
{
    CPU0,
    CPU1,
    CPU_ANY
} Cpu;

class Thread
{
    std::vector<ThreadSupport *> _actors;
    QueueSetHandle_t _queue_set;
    std::string _name;
    size_t _stack_size;
    TaskHandle_t _task_handle;
    bool _stop_thread = false;
    int _priority;
    Cpu _preferred_cpu;
    size_t _queue_set_size;

public:
    Thread(const char *name, size_t stack_size, size_t queue_set_size, int priority = 5, Cpu preferred_cpu = Cpu::CPU_ANY) : _name(name),
                                                                                                                             _stack_size(stack_size),
                                                                                                                             _priority(priority),
                                                                                                                             _preferred_cpu(preferred_cpu), _queue_set_size(queue_set_size)
    {
    }
    const char *name() { return _name.c_str(); }
    Res start();
    Res add_actor(ThreadSupport &actor);
    void run();
    void step();
    void handle_expired_timers();
};

typedef struct PropInfo
{
    const char *name;
    const char *type;
    const char *desc;
    const char *mode;
    Option<float> min;
    Option<float> max;
} PropInfo;

class PublishTxd : public Msg {
public:
    static uint32_t msg_id() { return FNV("PublishTxd"); };
    static const char* msg_name() { return "PublishTxd"; };

    JsonDocument doc;
    PublishTxd(const JsonDocument &doc) : doc(doc) {}
};

class PublishRxd : public Msg {
public:
    static uint32_t msg_id() { return FNV("PublishRxd"); };
    static const char* msg_name() { return "PublishRxd"; };
    JsonDocument doc;
    PublishRxd(const JsonDocument &doc) : doc(doc) {}
};

template <typename T>
void handle(JsonDocument &doc, const char *key, std::function<void(const T &)> f)
{
    if (doc.containsKey(key) && doc[key].is<T>())
    {
        T value = doc[key].as<T>();
        f(value);
    }
}

template <typename T>
void handle(std::optional<T> &v, std::function<void(const T &)> f)
{
    if (v)
        f(*v);
}

#endif
