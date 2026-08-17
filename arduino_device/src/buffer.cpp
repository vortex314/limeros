#include "buffer.h"

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
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < _index; ++i)
    {
        crc ^= static_cast<uint16_t>(_buffer[i]) << 8;
        for (uint8_t bit = 0; bit < 8; ++bit)
        {
            if (crc & 0x8000)
            {
                crc = static_cast<uint16_t>((crc << 1) ^ 0x1021);
            }
            else
            {
                crc = static_cast<uint16_t>(crc << 1);
            }
        }
    }
    return crc;
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
    size_t encoded_len = 0;
    uint8_t *buffer = data();
    if (data_len > _capacity)
        return ENOSPC;
    if (data_len == 0)
    {
        encoded_len = 1;
        buffer[0] = 0x00;
        return 0;
    }

    // COBS encode
    size_t write_pos = 0;
    size_t read_pos = 0;
    size_t block_start = 0;
    uint8_t code = 1;

    // Write placeholder for first code
    buffer[write_pos++] = 0x00;

    while (read_pos < data_len)
    {
        if (buffer[read_pos] == 0)
        {
            // Found a zero - finalize previous block
            buffer[block_start] = code;
            block_start = write_pos;
            code = 1;
            buffer[write_pos++] = 0x00; // Placeholder for next block
            ++read_pos;
        }
        else
        {
            // Copy non-zero byte
            buffer[write_pos++] = buffer[read_pos++];
            if (++code == 0xFF)
            {
                // Block full - finalize
                buffer[block_start] = code;
                block_start = write_pos;
                code = 1;
                buffer[write_pos++] = 0x00; // Placeholder
            }
        }
    }

    // Finalize last block
    buffer[block_start] = code;

    // Add final delimiter
    buffer[write_pos++] = 0x00;
    encoded_len = write_pos;
    resize(encoded_len);

    return 0;
}

//  // Decode with CRC verification
//  static bool cobs_decode_inplace(uint8_t* buffer, size_t encoded_len, size_t& data_len) {
int Buffer::cobs_decode_inplace()
{
    size_t encoded_len = size();
    uint8_t *buffer = data();
    if (encoded_len == 0)
        return ENOSPC;

    // Remove COBS encoding
    size_t read_pos = 0;
    size_t write_pos = 0;
    size_t total_decoded = 0;

    while (read_pos < encoded_len)
    {
        uint8_t code = buffer[read_pos++];
        if (code == 0)
            break; // End of frame

        // Copy code-1 bytes
        for (uint8_t i = 0; i < code - 1; ++i)
        {
            if (read_pos >= encoded_len)
                return ENOSPC;
            buffer[write_pos++] = buffer[read_pos++];
            ++total_decoded;
        }

        // Insert zero if needed
        if (code != 0xFF && read_pos < encoded_len && buffer[read_pos] != 0)
        {
            // But wait - if the next byte is not zero, this zero is implicit
            // Only insert if we're not at the end and the next byte isn't a delimiter
            // Actually, in COBS, if code != 0xFF, a zero is implicit
            // UNLESS we're at the end of the frame
            if (read_pos < encoded_len && buffer[read_pos] != 0x00)
            {
                buffer[write_pos++] = 0x00;
                ++total_decoded;
            }
        }
    }

    // Remove CRC bytes (last 2 bytes of data)
    if (total_decoded < 2)
        return ENOSPC;
    resize(total_decoded - 2);

    return 0;
}
