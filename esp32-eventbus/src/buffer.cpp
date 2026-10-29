#include "buffer.h"
#include "cobs_codec.h"

Buffer::Buffer(size_t capacity)
{
    _buffer = (uint8_t *)malloc(capacity);
    _capacity = capacity;
    _index = 0;
    _own_memory = true;
}
Buffer::Buffer(std::vector<uint8_t> vec)
{
    _capacity = vec.size();
    _buffer = (uint8_t *)malloc(_capacity);
    if (_buffer)
    {
        std::copy(vec.begin(), vec.end(), _buffer);
        _index = _capacity;
        _own_memory = true;
    }
    else
    {
        _capacity = 0;
        _index = 0;
        _own_memory = false;
        WARN("Failed to allocate memory in Buffer constructor");
    }
}
Buffer::~Buffer()
{
    if (_own_memory)
    {
        free(_buffer);
    }
}

void Buffer::capacity(size_t new_capacity)
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
                WARN("Failed to resize buffer in Buffer::capacity");
            }
        }
        else
        {
            WARN("Cannot resize non-owning buffer in Buffer::capacity");
        }
    }
    else
    {
        // Do nothing if new_capacity <= _capacity
    }
}

int Buffer::write(uint8_t b)
{
    if (_index < _capacity)
    {
        _buffer[_index++] = b;
        return 0;
    }
    WARN("Buffer overflow in Buffer::push");
    return ENOSPC;
}
int Buffer::write(const uint8_t *data, size_t len)
{
    if (_index + len <= _capacity)
    {
        memcpy(_buffer + _index, data, len);
        _index += len;
        return 0;
    }
    WARN("Buffer overflow in Buffer::write");
    return ENOSPC;
}

uint16_t Buffer::calculate_crc() const
{
    return crc16_ccitt(_buffer, _index);
}

int Buffer::add_crc()
{
    uint16_t crc = calculate_crc();
    uint8_t crc_bytes[2];
    crc_bytes[0] = static_cast<uint8_t>((crc >> 8) & 0xFF);
    crc_bytes[1] = static_cast<uint8_t>(crc & 0xFF);
    return write(crc_bytes, sizeof(crc_bytes));
}

bool Buffer::verify_crc_remove()
{
    if (_index < 2)
    {
        WARN("Buffer too small to contain CRC");
        return false;
    }
    uint16_t stored_crc = (static_cast<uint16_t>(_buffer[_index - 2]) << 8) |
                          static_cast<uint16_t>(_buffer[_index - 1]);
    Buffer temp_buffer(_buffer, _index - 2);
    uint16_t calculated_crc = temp_buffer.calculate_crc();
    if (stored_crc == calculated_crc)
    {
        _index -= 2;
        return true;
    }
    else
    {
        WARN("CRC verification failed");
        _index -= 2; // Remove the CRC bytes from the buffer
        return false;
    }
}

int Buffer::cobs_encode_inplace()
{
    size_t data_len = size();
    uint8_t *buffer = data();

    const size_t encoded_capacity = data_len + (data_len / 254) + 1;
    if (encoded_capacity + 1 > _capacity)
        return ENOSPC;

    std::vector<uint8_t> input(buffer, buffer + data_len);
    const size_t encoded_len = cobs_encode(input.data(), input.size(), buffer, encoded_capacity);
    if (encoded_len == 0)
        return ENOSPC;

    buffer[encoded_len] = 0x00;
    resize(encoded_len + 1);

    return 0;
}

//  // Decode with CRC verification
//  static bool cobs_decode_inplace(uint8_t* buffer, size_t encoded_len, size_t& data_len) {
int Buffer::cobs_decode_inplace()
{
    size_t encoded_len = size();
    uint8_t *buffer = data();
    if (encoded_len == 0 || buffer[encoded_len - 1] != 0x00)
        return ENOSPC;

    const int decoded_len = cobs_decode(buffer, encoded_len - 1, buffer, _capacity);
    if (decoded_len < 0)
        return ENOSPC;

    if (decoded_len < 2)
        return ENOSPC;
    resize(static_cast<size_t>(decoded_len));
    return verify_crc_remove() ? 0 : EBADMSG;
}
