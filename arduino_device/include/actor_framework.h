#pragma once

#include <Arduino.h>
#include <string.h> // for strncpy

typedef uint32_t ActorId;
typedef uint32_t MsgType;
typedef uint32_t TimerId;

#include "circ_buf.h"
#include <array>
#include <memory>
#include <fnv.h>
#include <vector>
#include <log.h>
#include <typed_message.h>

class ActorSystem;


#define DEFINE_MSG(Name, ...)                                      \
    class Name : public TypedMessage                                    \
    {                                                              \
    public:                                                        \
        static constexpr MsgType MSG_ID = FNV(#Name);         \
        static constexpr const char *MSG_NAME = #Name;             \
        virtual MsgType msg_id() const { return MSG_ID; };        \
        virtual const char *msg_name() const { return MSG_NAME; }; \
        virtual ~Name() = default;                                 \
        __VA_ARGS__                                                \
    };

// ============================================================
// ActorMessage
// ============================================================
struct ActorMessage
{
    friend class ActorSystem;

public:
    ActorId src = 0;
    ActorId dst = 0;
    TypedMessage *payload = nullptr;

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
    void set_payload(TypedMessage *p)
    {
        if (payload)
            delete payload;
        payload = p;
    }
};

// ============================================================
// Actor base class
// ============================================================
class Actor
{
public:
    uint8_t id = 0;

    virtual void setup() {}
    virtual void receive(std::shared_ptr<ActorMessage> msg) = 0;
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

#ifndef POOL_SIZE
#define POOL_SIZE 16
#endif

class ActorSystem
{
    friend class ActorMessage;

public:
    ActorSystem(const ActorSystem &) = delete;
    ActorSystem &operator=(const ActorSystem &) = delete;

private:
    std::vector<Actor *> _actors;
    CircBuf<std::shared_ptr<ActorMessage>> _circ_buf;

public:
    ActorSystem() : _circ_buf(MAX_MESSAGES)
    {
        _actors.reserve(MAX_ACTORS);

    };
    uint8_t registerActor(Actor *actor)
    {
        if (!actor || _actors.size() >= MAX_ACTORS)
            return 0;

        actor->id = _actors.size();
        _actors.push_back(actor);
        actor->setup();
        return actor->id;
    }

    bool send(std::shared_ptr<ActorMessage> msg)
    {
        if (_circ_buf.hasSpace() == false)
        {
            WARN("ActorSystem message queue full, dropping message");
            return false;
        }
        return _circ_buf.write(msg) == 0 ? true : false;
    }

    bool send(ActorId dst, ActorId src, TypedMessage* payload)
    {
        auto msg = std::make_shared<ActorMessage>();
        msg->src = src;
        msg->dst = dst;
        msg->set_payload(payload);
        return send(msg);
    }

    void processOne()
    {
        if (_circ_buf.isEmpty())
            return;

        auto msg = _circ_buf.read();

        if (msg && msg->dst >= 0 && msg->dst < _actors.size() && _actors[msg->dst])
            _actors[msg->dst]->receive(msg);
    }

    void processAll()
    {
        while (_circ_buf.hasData())
            processOne();
    }

    void updateAll()
    {
        for (uint8_t i = 0; i < _actors.size(); ++i)
            if (_actors[i])
                _actors[i]->update();
    }

    void run()
    {
        processAll();
        updateAll();
    }

    uint8_t actorCount() const { return _actors.size(); }
    uint8_t queueSize() const { return _circ_buf.size(); }
};

// Global instance
extern ActorSystem actor_system;