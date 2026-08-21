#ifndef MSG_H
#define MSG_H
#include "app_compat.h"
#include <option.h>
#include <log.h>

#include <buffer.h>
#include <fnv.h>
#include <typed_message.h>

typedef std::vector<uint8_t> Bytes;
extern void print_cbor_diagnostic(const uint8_t *data, size_t size);

// ---------------------- abstract class Msg ------------------

class SerdesMsg 
{
public:
    virtual ~SerdesMsg() = default;
    virtual int encode(Buffer &buffer) const = 0;
    virtual int decode(Buffer &buffer) = 0;
};

class Msg : public TypedMsg,public SerdesMsg
{
public:
    static const uint32_t MSG_ID = FNV("Msg");
    static constexpr const char *MSG_NAME = "Msg";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };

    virtual ~Msg() = default;
    virtual int encode(Buffer &buffer) const
    {
        ERROR("encode not implemented for %s", msg_name());
        return ENOTSUP;
    }
    virtual int decode(Buffer &buffer)
    {
        WARN("decode not implemented for %s", msg_name());
        return ENOTSUP;
    }

    template <typename T>
    void handle_if(std::function<void(const T &)> f) const
    {
        if (msg_id() == T::MSG_ID)
            f(static_cast<const T &>(*this));
    }

};

#endif // MSG_H
