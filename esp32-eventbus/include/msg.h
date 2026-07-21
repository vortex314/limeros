
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
extern void print_cbor_diagnostic(const uint8_t *data, size_t size);
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

class Buffer {
    private :
        uint8_t* _buffer;
        size_t _capacity;
        size_t _index;
        bool _own_memory = false;

    public :
        Buffer(uint8_t* buffer, size_t capacity) : _buffer(buffer), _capacity(capacity), _index(0) {}
        Buffer(size_t capacity) {
            _buffer = (uint8_t*)malloc(capacity);
            _capacity = capacity;
            _index = 0;
            _own_memory = true;     
        }
        Buffer (std::vector<uint8_t> vec) {
            _capacity = vec.size();
            _buffer = (uint8_t*)malloc(_capacity);
            if (_buffer) {
                std::copy(vec.begin(), vec.end(), _buffer);
                _index = _capacity;
                _own_memory = true;
            } else {
                _capacity = 0;
                _index = 0;
                _own_memory = false;
                WARN("Failed to allocate memory in Buffer constructor");
            }
        }
        ~Buffer() {
            if (_own_memory) {
                free(_buffer);
            }
        }
        uint8_t* data() const { return _buffer; };
        size_t size() const { return _index; }
        uint8_t operator[](size_t index) const { assert(index < _index); return _buffer[index]; }
        uint8_t& operator[](size_t index) { assert(index < _index); return _buffer[index]; }
        void clear() { _index = 0; }
        void resize(size_t new_size) {
            assert(new_size <= _capacity);
            _index = new_size;
        }
        void capacity(size_t new_capacity) {
            if (new_capacity > _capacity) {
                if (_own_memory) {
                    uint8_t* new_buffer = (uint8_t*)realloc(_buffer, new_capacity);
                    if (new_buffer) {
                        _buffer = new_buffer;
                        _capacity = new_capacity;
                    } else {
                        WARN("Failed to resize buffer in Buffer::capacity");
                    }
                } else {
                    WARN("Cannot resize non-owning buffer in Buffer::capacity");
                }
            } else {
                // Do nothing if new_capacity <= _capacity
            }
        }
        std::vector<uint8_t> to_vector() const {
            return std::vector<uint8_t>(_buffer, _buffer + _index);
        }
        size_t capacity() const { return _capacity; }
        int push_back(uint8_t b) {
            if (_index < _capacity) {
                _buffer[_index++] = b;
                return 0;
            }
            WARN("Buffer overflow in Buffer::push");
            return ENOSPC;
        }
};

// ---------------------- abstract class Msg ------------------

class Msg
{
public:
    static const uint32_t MSG_ID = FNV("Msg");
    static constexpr const char *MSG_NAME ="Msg";

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
    bool is() const { return msg_id() == T::MSG_ID  ; }
    virtual int encode(Buffer &buffer) const
    {
        ERROR("encode not implemented for %s", msg_name());
        return ENOTSUP;
    }
    virtual int decode(const Buffer &buffer)
    {
        WARN("decode not implemented for %s", msg_name());
        return ENOTSUP;
    }
};


