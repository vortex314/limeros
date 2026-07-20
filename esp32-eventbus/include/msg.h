
#include <result.h>
#include <option.h>
#include <cbor.h>
#include <errno.h>
#include <log.h>

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
/* class Bytes
{
private:
    uint8_t *_buffer;
    size_t _capacity;
    size_t _index;
    bool _own_memory = false;

public:
    Bytes(uint8_t *buffer, size_t capacity) : _buffer(buffer), _capacity(capacity), _index(0) {}
    Bytes(size_t capacity)
    {
        _buffer = (uint8_t *)malloc(capacity);
        _capacity = capacity;
        _index = 0;
        _own_memory = true;
    }
    ~Bytes()
    {
        if (_own_memory)
        {
            free(_buffer);
        }
    }
    uint8_t *data() const { return _buffer; };
    size_t size() const { return _index; }
    uint8_t operator[](size_t index) const { return _buffer[index]; }
    uint8_t &operator[](size_t index) { return _buffer[index]; }
    void clear() { _index = 0; }
    void resize(size_t new_size)
    {
        if (_index > new_size)
        {
            _index = new_size;
        }
    }
    void capacity(size_t new_capacity)
    {
        if (new_capacity > _capacity)
        {
            if (_own_memory)
            {
                uint8_t *new_buffer = (uint8_t *)realloc(_buffer, new_capacity);
                if (new_buffer)
                {
                    _buffer = new_buffer;
                    _capacity = new_capacity;
                }
                else
                {
                    WARN("Failed to resize buffer in Bytes::capacity");
                }
            }
            else
            {
                WARN("Cannot resize non-owning buffer in Bytes::capacity");
            }
        }
        else
        {
            WARN("Resize exceeds capacity in Bytes::resize");
        }
    }

    int push(uint8_t b)
    {
        if (_index < _capacity)
        {
            _buffer[_index++] = b;
            return 0;
        }
        WARN("Buffer overflow in Bytes::push");
        return ENOSPC;
    }

    std::vector<uint8_t> to_vector() const
    {
        return std::vector<uint8_t>(_buffer, _buffer + _index);
    }

};
*/

// ---------------------- abstract class Msg ------------------

class Msg
{
public:
    static uint32_t msg_id() { return FNV("Msg"); };
    static const char *msg_name() { return "Msg"; };
    template <typename T>
    void handle_if(std::function<void(const T &)> f) const
    {
        if (msg_id() == T::msg_id())
            f(static_cast<const T &>(*this));
    }
    template <typename T>
    bool is() const { return msg_id() == T::msg_id(); }
    int encode(Bytes &buffer)
    {
        ERROR("encode not implemented for %s", msg_name());
        return ENOTSUP;
    }
    int decode(const Bytes &buffer)
    {
        WARN("decode not implemented for %s", msg_name());
        return ENOTSUP;
    }
};

