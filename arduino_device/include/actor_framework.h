#pragma once

#include <Arduino.h>
#include <string.h> // for strncpy

typedef uint32_t ActorId;
typedef uint32_t MsgType;
typedef uint32_t TimerId;

#include "messages.h"
#include "circ_buf.h"
#include <array>
#include <memory>

class ActorSystem;

// ============================================================
// ActorMessage
// ============================================================
struct ActorMessage
{
    friend class ActorSystem;

public:
    ActorId src = 0;
    ActorId dst = 0;

private:
    MsgType msg_type = 0;
    void *payload = nullptr;

public:
    // ---- type-safe helpers ----
    template <typename T>
    bool is() const { return msg_type == T::MSG_ID; }
    template <typename T>
    T *as() { return static_cast<T *>(payload); }

    // handle() – returns true if the message was of type T
    template <typename T, typename Func>
    bool handle(Func &&func) const
    {
        if (msg_type == T::MSG_ID)
        {
            func(*static_cast<const T *>(payload));
            return true;
        }
        return false;
    }
    template <typename T>
    void set_payload(const T *p)
    {
        payload = p;
        msg_type = T::MSG_ID;
    }

    ~ActorMessage()
    {
        delete static_cast<MessagePayload *>(payload);
    };
};

// ============================================================
// Actor base class
// ============================================================
class Actor
{
public:
    uint8_t id = 0;

    virtual void setup() {}
    virtual void receive(const ActorMessage &msg) = 0;
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
    Actor *actors[MAX_ACTORS + 1] = {};
    uint8_t numActors = 0;
    ActorMessage pool[POOL_SIZE] = {};
    bool bitmap_acquired[POOL_SIZE] = {};
    CircBuf<ActorMessage *> queue{MAX_MESSAGES};

public:
    ActorSystem() {

    };
    uint8_t registerActor(Actor *actor)
    {
        if (!actor || numActors >= MAX_ACTORS)
            return 0;

        actor->id = numActors++;
        actors[actor->id] = actor;
        actor->setup();
        return actor->id;
    }

    bool send(const ActorMessage *msg)
    {
        if (queue.hasSpace() == false)
        {
            release(const_cast<ActorMessage *>(msg));
            return false;
        }
        return queue.write(const_cast<ActorMessage *>(msg)) == 0 ? true : false;
    }

    template <typename T>
    bool send(ActorId dst, ActorId src, T *payload)
    {
        ActorMessage &msg = acquire();
        msg.src = src;
        msg.dst = dst;
        msg.payload = payload;
        msg.msg_type = T::MSG_ID;
        return send(&msg);
    }

    void processOne()
    {
        if (queue.isEmpty())
            return;

        ActorMessage *msg = queue.read();

        if (msg && msg->dst >= 0 && msg->dst <= numActors && actors[msg->dst])
            actors[msg->dst]->receive(*msg);
        if (msg->payload != nullptr)
        {
            delete (msg->payload);
            msg->payload = nullptr;
        }
        release(msg);
    }

    void processAll()
    {
        while (queue.hasData())
            processOne();
    }

    void updateAll()
    {
        for (uint8_t i = 0; i < numActors; ++i)
            if (actors[i])
                actors[i]->update();
    }

    void run()
    {
        processAll();
        updateAll();
    }

    uint8_t actorCount() const { return numActors; }
    uint8_t queueSize() const { return queue.size(); }

    ActorMessage &acquire()
    {

        for (size_t i = 0; i < POOL_SIZE; ++i)

            if (!bitmap_acquired[i])
            {
                bitmap_acquired[i] = true;
                return pool[i];
            }

        return pool[0]; // fallback, should not happen if POOL_SIZE is sufficient
    }
    void release(ActorMessage *msg)
    {
        if (!msg)
            return;
        if (msg->payload != nullptr)
        {
            delete static_cast<MessagePayload *>(msg->payload);
            msg->payload = nullptr;
        }

        size_t index = msg - pool;
        if (index < POOL_SIZE)
        {
            bitmap_acquired[index] = false;
        }
    }
};

// Global instance
extern ActorSystem actor_system;