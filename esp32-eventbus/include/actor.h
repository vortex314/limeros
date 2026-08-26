#ifndef ACTOR_H
#define ACTOR_H
#include <freertos/FreeRTOS.h>
#include <freertos/mpu_wrappers.h>
#include <freertos/portmacro.h>
#include <freertos/queue.h>
#include <freertos/timers.h>
#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <functional>
#include <esp_timer.h>
#include <log.h>
#include <util.h>
#include <option.h>
#include <result.h>
#include <memory>
#include <stdint.h>
#include <msgs.h>
#include <unordered_map>
#include <typed_message.h>

typedef uint32_t ActorId;
typedef uint32_t MsgType;
typedef uint32_t TimerId;
class ActorPayload : public TypedMsg
{
};

uint64_t current_time();

// ============================================================
// ActorMessage
// ============================================================
struct ActorMessage
{
    friend class ActorSystem;

public:
    ActorId src = 0;
    ActorId dst = 0;
    ActorPayload *payload = nullptr;

public:
    // ---- type-safe helpers ----
    template <typename T>
    bool is() const { return payload->msg_id() == T::MSG_ID; }
    template <typename T>
    T *as() { return static_cast<T *>(payload); }

    // handle() – returns true if the message was of type T
    template <typename T, typename Func>
    bool handle(Func &&func) const
    {
        if (payload->msg_id() == T::MSG_ID)
        {
            func(*static_cast<const T *>(payload));
            return true;
        }
        return false;
    }

    ~ActorMessage()
    {
        if (payload)
            delete payload;
    }
    void set_payload(ActorPayload *p)
    {
        if (payload)
            delete payload;
        payload = p;
    }
};

class TimerMsg : public ActorPayload
{
TYPED_MSG(TimerMsg)
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
    ActorId _actor_id;
    int _timer_id;
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
    virtual void setup() = 0;
    virtual void on_stop() = 0;
    virtual QueueHandle_t queue_handle() = 0;
    Option<QueueHandle_t> additional_queue() { return Option<QueueHandle_t>(); }
    virtual uint64_t sleep_time() = 0;
    virtual void handle_expired_timers() = 0;
    virtual const char *name() = 0;
    virtual ~ThreadSupport() = default;
};

class Actor;

class EventBus : public Queue<const ActorMessage *>
{
    std::unordered_map<uint32_t, Actor *> _actors;
    ActorId _next_actor_id = 1;
    std::vector<std::function<void(const ActorMessage &)>> _message_handlers;
    size_t _stack_size = 1024;
    TaskHandle_t _task_handle;


public:
    EventBus(size_t size);
    ~EventBus();
    void push(const ActorMessage *env);
    void register_actor(const char *name, Actor * actor);
    void register_handler(std::function<void(const ActorMessage &)> handler)
    {
        _message_handlers.push_back(handler);
    }
    void loop();
    void start();
    ActorId find_actor(const char *name);
    const char *find_actor_name(ActorId id);
    const std::unordered_map<uint32_t, Actor *> &actors() { return _actors; }
};

class EventBus;

class Actor
{
private:
    ActorId _self;
    Timers _timers;
    EventBus *_eventbus;

public:
    Actor() { };
    ~Actor() { INFO("Destroying actor %s", name()); }

    virtual void setup() { INFO("actor %s default started.", _eventbus->find_actor_name(_self)); }
    virtual void on_stop() { INFO("actor %s default stopped.", _eventbus->find_actor_name(_self)); }
    virtual void receive(const ActorMessage &env)
    {
        WARN(" No message handler for actor %s ", _eventbus->find_actor_name(_self));
    }

    /// Declare that this actor handles messages of the given type name.
    /// Call in the constructor. If no interests are registered, the actor
    /// receives all messages (backward-compatible default).

    /// Returns true if this actor should receive messages of the given type.
    /// An empty interest list means "accept all".

    std::vector<int> get_expired_timers() { return _timers.get_expired_timers(); }
    Timers &timers() { return _timers; }
    uint64_t sleep_time() { return _timers.sleep_time(); }
    ActorId ref() const { return _self; }
    const char *name() { return _eventbus->name(); }
    EventBus &eventbus() const { return *_eventbus; }

    int tell(ActorId dst,  ActorPayload *payload)
    {
        ActorMessage *env = new ActorMessage();
        env->src = _self;
        env->dst = dst;
        env->payload = payload;
        _eventbus->push(env);
        return 0; // Added return statement to match the int return type
    }

    void emit(const ActorPayload *env);
    void set_eventbus(ActorId id, EventBus *eventbus)
    {
        _self = id;
        _eventbus = eventbus;
    }

    void handle_expired_timers()
    {
        for (int id : _timers.get_expired_timers())
        {
            ActorMessage *env = new ActorMessage();
            env->payload =  new TimerMsg(id);
            env->src = _self;
            env->dst = _self;
            _eventbus->push(env);
            _timers.refresh(id);
        }
    };
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

#endif
