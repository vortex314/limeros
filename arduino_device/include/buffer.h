#ifndef BUFFER_H
#define BUFFER_H
#include <app_compat.h>
#include "log.h"

class Buffer
{
private:
    uint8_t *_buffer;
    size_t _capacity;
    size_t _index;
    bool _own_memory = false;

public:
    Buffer(uint8_t *buffer, size_t capacity) : _buffer(buffer), _capacity(capacity), _index(0) {}
    Buffer(size_t capacity);
    Buffer(std::vector<uint8_t> vec);
    ~Buffer();
    uint8_t *data() const { return _buffer; };
    size_t size() const { return _index; }
    uint8_t operator[](size_t index) const
    {
        assert(index < _index);
        return _buffer[index];
    }
    uint8_t &operator[](size_t index)
    {
        assert(index < _index);
        return _buffer[index];
    }
    void clear() { _index = 0; }
    void resize(size_t new_size)
    {
        assert(new_size <= _capacity);
        _index = new_size;
    }
    void capacity(size_t new_capacity);
    ;
    std::vector<uint8_t> to_vector() const
    {
        return std::vector<uint8_t>(_buffer, _buffer + _index);
    }
    size_t capacity() const { return _capacity; }
    int write(uint8_t b);
    int write(const uint8_t *data, size_t len);
    uint16_t calculate_crc() const;
    int add_crc();
    bool verify_crc_remove() ;
    int cobs_encode_inplace();
    int cobs_decode_inplace(); // Returns 0 on success, ENOSPC on failure
};

#endif // BUFFER_H