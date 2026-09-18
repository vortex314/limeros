#ifndef TYPED_MESSAGE_H
#define TYPED_MESSAGE_H
#include <stdint.h>
#include <fnv.h>
#include <atomic>
class TypedMessage
{
public:
    static const uint32_t MSG_ID = FNV("Message");
    static constexpr const char *MSG_NAME = "Message";

    virtual ~TypedMessage() = default;
    virtual uint32_t msg_id() const = 0;
    virtual const char *msg_name() const = 0;
};

#define TYPED_MESSAGE(MSG_TYPE) \
public: \
    static constexpr const char *MSG_NAME = #MSG_TYPE; \
    static const uint32_t MSG_ID = FNV(MSG_NAME); \
    uint32_t msg_id() const override { return MSG_ID; } \
    const char *msg_name() const override { return MSG_NAME; }


class AllocMessage {
    virtual void release() = 0;
    virtual void acquire() = 0;
};

class SharedMessage : public AllocMessage {
    std::atomic<int> ref_count{1};
public:
    void release() override {
        if (--ref_count == 0) {
            delete this;
        }
    }
    void acquire() override {
        ++ref_count;
    }
    virtual ~SharedMessage() = default;
};

class OneShotMessage : public AllocMessage {
    std::atomic<bool> released{false};  
public:
    void release() override {
        bool expected = false;
        if (released.compare_exchange_strong(expected, true)) {
            delete this;
        }
    }
    void acquire() override {
        // No-op for one-shot message
    }
    virtual ~OneShotMessage() = default;
};

#endif // TYPED_MESSAGE_H