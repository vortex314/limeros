#ifndef MSG_H
#define MSG_H
#include <option.h>
#include <errno.h>
#include <log.h>
#include <assert.h>
#include <vector>
#include <cstdlib>
#include <buffer.h>
// FNV-1a hash function for 32-bit hash value
constexpr uint32_t fnv1a_32_1(const char *str, uint32_t hash = 2166136261U)
{
    return *str == '\0' ? hash : fnv1a_32_1(str + 1, (hash ^ static_cast<uint32_t>(*str)) * 16777619U);
}

// Helper to compute the hash at compile time for a string literal
template <std::size_t N>
constexpr uint32_t FNV(const char (&str)[N])
{
    return fnv1a_32_1(str);
}
typedef std::vector<uint8_t> Bytes;
extern void print_cbor_diagnostic(const uint8_t *data, size_t size);


// ---------------------- abstract class Msg ------------------

class Msg
{
public:
    static const uint32_t MSG_ID = FNV("Msg");
    static constexpr const char *MSG_NAME = "Msg";

    virtual uint32_t msg_id() const { return MSG_ID; };
    virtual const char *msg_name() const { return MSG_NAME; };

    virtual ~Msg() = default;

    template <typename T>
    void handle_if(std::function<void(const T &)> f) const
    {
        if (msg_id() == T::MSG_ID)
            f(static_cast<const T &>(*this));
    }
    template <typename T>
    bool is() const { return msg_id() == T::MSG_ID; }
    virtual int encode(Buffer &buffer) const
    {
        ERROR("encode not implemented for %s", msg_name());
        return ENOTSUP;
    }
    virtual int decode( Buffer &buffer)
    {
        WARN("decode not implemented for %s", msg_name());
        return ENOTSUP;
    }
};

#endif // MSG_H
