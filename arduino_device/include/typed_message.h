#ifndef TYPED_MESSAGE_H
#define TYPED_MESSAGE_H
#include <stdint.h>
#include <fnv.h>
class TypedMessage
{
public:
    static const uint32_t MSG_ID = FNV("Message");
    static constexpr const char *MSG_NAME = "Message";

    virtual ~TypedMessage() = default;
    virtual uint32_t msg_id() const = 0;
};

#endif // TYPED_MESSAGE_H