#include "cbor_codec.h"

#include <string.h>

CborEncoder::CborEncoder(uint8_t *buffer, size_t capacity)
    : buf(buffer), cap(capacity), len(0), _last_error(0) {}

bool CborEncoder::ok() const
{
  return _last_error == 0;
}

int CborEncoder::error() const
{
  return _last_error;
}

size_t CborEncoder::size() const
{
  return len;
}

const uint8_t *CborEncoder::data() const
{
  return buf;
}

CborEncoder &CborEncoder::beginMap(uint32_t count)
{
  if (_last_error != 0)
    return *this;
  writeHead(5, count);
  return *this;
}

CborEncoder &CborEncoder::beginArray(uint32_t count)
{
  if (_last_error != 0)
    return *this;
  writeHead(4, count);
  return *this;
}

CborEncoder &CborEncoder::write(uint32_t value)
{
  if (_last_error != 0)
    return *this;
  writeHead(0, value);
  return *this;
}

CborEncoder &CborEncoder::write(const char *text)
{
  if (_last_error != 0)
    return *this;
  size_t n = strlen(text);
  if (!writeHead(3, (uint32_t)n))
    return *this;
  putBytes((const uint8_t *)text, n);
  return *this;
}

CborEncoder &CborEncoder::write(bool value)
{
  if (_last_error != 0)
    return *this;
  putByte(value ? 0xF5 : 0xF4);
  return *this;
}

CborEncoder &CborEncoder::writeNull()
{
  if (_last_error != 0)
    return *this;
  putByte(0xF6);
  return *this;
}

CborEncoder &CborEncoder::write(const uint8_t *p, size_t n)
{
  if (_last_error != 0)
    return *this;
  if (!writeHead(2, (uint32_t)n))
    return *this;
  putBytes(p, n);
  return *this;
}

CborEncoder &CborEncoder::write(const std::vector<uint8_t> &v)
{
  return write(v.data(), v.size());
}

CborEncoder &CborEncoder::write(int32_t value)
{
  if (_last_error != 0)
    return *this;
  // Firmware fields are non-negative in practice; no CBOR negative-int support.
  writeHead(0, (uint32_t)value);
  return *this;
}

CborEncoder &CborEncoder::write(uint64_t value)
{
  if (_last_error != 0)
    return *this;
  // This firmware only needs values that fit in 32 bits.
  writeHead(0, (uint32_t)value);
  return *this;
}

CborEncoder &CborEncoder::write(float value)
{
  if (_last_error != 0)
    return *this;
  putFloat32(value);
  return *this;
}

CborEncoder &CborEncoder::write(const std::string &text)
{
  return write(text.c_str());
}

CborEncoder &CborEncoder::write(const std::vector<uint32_t> &v)
{
  if (_last_error != 0)
    return *this;
  if (!writeHead(4, (uint32_t)v.size()))
    return *this;
  for (uint32_t item : v)
  {
    write(item);
    if (_last_error != 0)
      return *this;
  }
  return *this;
}

bool CborEncoder::putByte(uint8_t b)
{
  if (_last_error != 0)
    return false;
  if (len >= cap)
  {
    _last_error = 1;
    return false;
  }
  buf[len++] = b;
  return true;
}

bool CborEncoder::putBytes(const uint8_t *p, size_t n)
{
  if (_last_error != 0)
    return false;
  if (n > cap - len)
  {
    _last_error = 1;
    return false;
  }
  if (n > 0)
  {
    memcpy(buf + len, p, n);
    len += n;
  }
  return true;
}

bool CborEncoder::putU16(uint16_t v)
{
  uint8_t b[2];
  b[0] = (uint8_t)(v >> 8);
  b[1] = (uint8_t)(v & 0xFF);
  return putBytes(b, 2);
}

bool CborEncoder::putU32(uint32_t v)
{
  uint8_t b[4];
  b[0] = (uint8_t)(v >> 24);
  b[1] = (uint8_t)(v >> 16);
  b[2] = (uint8_t)(v >> 8);
  b[3] = (uint8_t)(v & 0xFF);
  return putBytes(b, 4);
}

bool CborEncoder::putFloat32(float v)
{
  uint32_t bits;
  memcpy(&bits, &v, sizeof(bits));
  if (!putByte(0xFA))
    return false;
  return putU32(bits);
}

bool CborEncoder::writeHead(uint8_t major, uint32_t value)
{
  uint8_t mt = (uint8_t)(major << 5);

  if (value < 24)
  {
    return putByte((uint8_t)(mt | value));
  }

  if (value <= 0xFF)
  {
    return putByte((uint8_t)(mt | 24)) && putByte((uint8_t)value);
  }

  if (value <= 0xFFFF)
  {
    return putByte((uint8_t)(mt | 25)) && putU16((uint16_t)value);
  }

  return putByte((uint8_t)(mt | 26)) && putU32(value);
}

static bool cborSkipItem(const uint8_t *data, size_t len, size_t &pos, int depth);

static bool cborReadHeader(const uint8_t *data,
                           size_t len,
                           size_t &pos,
                           uint8_t &major,
                           uint32_t &value)
{
  if (pos >= len)
  {
    return false;
  }

  uint8_t initial = data[pos++];
  major = (uint8_t)(initial >> 5);
  uint8_t ai = (uint8_t)(initial & 0x1F);

  if (ai < 24)
  {
    value = ai;
    return true;
  }

  if (ai == 24)
  {
    if (pos + 1 > len)
    {
      return false;
    }

    value = data[pos];
    pos += 1;
    return true;
  }

  if (ai == 25)
  {
    if (pos + 2 > len)
    {
      return false;
    }

    value = ((uint32_t)data[pos] << 8) |
            (uint32_t)data[pos + 1];
    pos += 2;
    return true;
  }

  if (ai == 26)
  {
    if (pos + 4 > len)
    {
      return false;
    }

    value = ((uint32_t)data[pos] << 24) |
            ((uint32_t)data[pos + 1] << 16) |
            ((uint32_t)data[pos + 2] << 8) |
            (uint32_t)data[pos + 3];
    pos += 4;
    return true;
  }

  if (ai == 27)
  {
    if (pos + 8 > len)
    {
      return false;
    }

    uint32_t hi = ((uint32_t)data[pos + 0] << 24) |
                  ((uint32_t)data[pos + 1] << 16) |
                  ((uint32_t)data[pos + 2] << 8) |
                  (uint32_t)data[pos + 3];

    uint32_t lo = ((uint32_t)data[pos + 4] << 24) |
                  ((uint32_t)data[pos + 5] << 16) |
                  ((uint32_t)data[pos + 6] << 8) |
                  (uint32_t)data[pos + 7];

    // This firmware does not need exact 64-bit values.
    value = (hi == 0) ? lo : 0xFFFFFFFFu;
    pos += 8;
    return true;
  }

  // Indefinite length and break are not supported.
  return false;
}

static bool cborSkipContent(uint8_t major,
                            uint32_t value,
                            const uint8_t *data,
                            size_t len,
                            size_t &pos,
                            int depth)
{
  if (depth > 8)
  {
    return false;
  }

  switch (major)
  {
  case 0: // unsigned integer
  case 1: // negative integer
  case 7: // simple values / floats
    return true;

  case 2: // byte string
  case 3: // text string
    if (value > len - pos)
    {
      return false;
    }

    pos += value;
    return true;

  case 4: // array
    if (value > len - pos)
    {
      return false;
    }

    for (uint32_t i = 0; i < value; ++i)
    {
      if (!cborSkipItem(data, len, pos, depth + 1))
      {
        return false;
      }
    }

    return true;

  case 5: // map
    if (value > (len - pos) / 2)
    {
      return false;
    }

    for (uint32_t i = 0; i < value; ++i)
    {
      if (!cborSkipItem(data, len, pos, depth + 1))
      {
        return false;
      }

      if (!cborSkipItem(data, len, pos, depth + 1))
      {
        return false;
      }
    }

    return true;

  case 6: // tag
    return cborSkipItem(data, len, pos, depth + 1);

  default:
    return false;
  }
}

static bool cborSkipItem(const uint8_t *data,
                         size_t len,
                         size_t &pos,
                         int depth)
{
  uint8_t major;
  uint32_t value;

  if (!cborReadHeader(data, len, pos, major, value))
  {
    return false;
  }

  return cborSkipContent(major, value, data, len, pos, depth);
}

bool cborFindBool(const uint8_t *data,
                  size_t len,
                  const char *key,
                  bool &out)
{
  size_t pos = 0;
  uint8_t major;
  uint32_t count;

  if (!cborReadHeader(data, len, pos, major, count))
  {
    return false;
  }

  if (major != 5)
  {
    return false;
  }

  if (count > len)
  {
    return false;
  }

  size_t keyLen = strlen(key);

  for (uint32_t i = 0; i < count; ++i)
  {
    uint8_t keyMajor;
    uint32_t keyInfo;

    if (!cborReadHeader(data, len, pos, keyMajor, keyInfo))
    {
      return false;
    }

    bool match = false;

    if (keyMajor == 3)
    {
      if (keyInfo > len - pos)
      {
        return false;
      }

      if (keyInfo == (uint32_t)keyLen &&
          memcmp(data + pos, key, keyLen) == 0)
      {
        match = true;
      }

      pos += keyInfo;
    }
    else
    {
      if (!cborSkipContent(keyMajor, keyInfo, data, len, pos, 1))
      {
        return false;
      }
    }

    uint8_t vMajor;
    uint32_t vInfo;

    if (!cborReadHeader(data, len, pos, vMajor, vInfo))
    {
      return false;
    }

    if (match)
    {
      // CBOR booleans: false = 0xF4, true = 0xF5
      if (vMajor == 7 && (vInfo == 20 || vInfo == 21))
      {
        out = (vInfo == 21);
        return true;
      }

      // Unsigned integer: 0 is false, non-zero is true.
      if (vMajor == 0)
      {
        out = (vInfo != 0);
        return true;
      }

      // Negative integer: treat as true if present.
      if (vMajor == 1)
      {
        out = true;
        return true;
      }
    }

    if (!cborSkipContent(vMajor, vInfo, data, len, pos, 1))
    {
      return false;
    }
  }

  return false;
}

// ---------------------------------------------------------------------------
// CborDecoder
// ---------------------------------------------------------------------------

CborDecoder::CborDecoder(const uint8_t *buffer, size_t length)
    : buf(buffer), len(length), pos(0), _last_error(0) {}

bool CborDecoder::ok() const
{
  return _last_error == 0;
}

int CborDecoder::error() const
{
  return _last_error;
}

size_t CborDecoder::offset() const
{
  return pos;
}

CborDecoder &CborDecoder::beginArray(uint32_t count)
{
  if (_last_error != 0)
    return *this;
  uint8_t major = 0;
  uint32_t actual = 0;
  if (!readHeader(major, actual))
    return *this;
  if (major != 4 || actual != count)
    _last_error = 1;
  return *this;
}

CborDecoder &CborDecoder::beginMap(uint32_t count)
{
  if (_last_error != 0)
    return *this;
  uint8_t major = 0;
  uint32_t actual = 0;
  if (!readHeader(major, actual))
    return *this;
  if (major != 5 || actual != count)
    _last_error = 1;
  return *this;
}

CborDecoder &CborDecoder::read(uint32_t &out)
{
  uint8_t major = 0;
  uint32_t value = 0;
  if (!readHeader(major, value))
    return *this;
  if (major != 0)
  {
    _last_error = 1;
    return *this;
  }
  out = value;
  return *this;
}

CborDecoder &CborDecoder::read(int32_t &out)
{
  if (_last_error != 0)
    return *this;
  uint8_t major = 0;
  uint32_t value = 0;
  if (!readHeader(major, value))
    return *this;
  if (major != 0)
  {
    _last_error = 1;
    return *this;
  }
  out = (int32_t)value;
  return *this;
}

CborDecoder &CborDecoder::read(uint64_t &out)
{
  if (_last_error != 0)
    return *this;
  uint8_t major = 0;
  uint32_t value = 0;
  if (!readHeader(major, value))
    return *this;
  if (major != 0)
  {
    _last_error = 1;
    return *this;
  }
  out = value;
  return *this;
}

CborDecoder &CborDecoder::read(float &out)
{
  if (_last_error != 0)
    return *this;
  uint8_t major = 0;
  uint32_t value = 0;
  if (!readHeader(major, value))
    return *this;
  if (major != 7)
  {
    _last_error = 1;
    return *this;
  }
  memcpy(&out, &value, sizeof(out));
  return *this;
}

CborDecoder &CborDecoder::read(char *out, size_t maxLen)
{
  if (_last_error != 0)
    return *this;
  uint8_t major = 0;
  uint32_t n = 0;
  if (!readHeader(major, n))
    return *this;
  if (major != 3 || n >= maxLen || pos + n > len)
  {
    _last_error = 1;
    return *this;
  }
  memcpy(out, buf + pos, n);
  pos += n;
  out[n] = '\0';
  return *this;
}

CborDecoder &CborDecoder::read(bool &out)
{
  if (_last_error != 0)
    return *this;
  if (pos >= len)
  {
    _last_error = 1;
    return *this;
  }
  uint8_t b = buf[pos];
  if (b == 0xF4)
  {
    out = false;
    ++pos;
    return *this;
  }
  if (b == 0xF5)
  {
    out = true;
    ++pos;
    return *this;
  }
  _last_error = 1;
  return *this;
}

CborDecoder &CborDecoder::readNull()
{
  if (_last_error != 0)
    return *this;
  if (pos >= len || buf[pos] != 0xF6)
  {
    _last_error = 1;
    return *this;
  }
  ++pos;
  return *this;
}

CborDecoder &CborDecoder::read(uint8_t *out, size_t cap, size_t &outLen)
{
  if (_last_error != 0)
    return *this;
  uint8_t major = 0;
  uint32_t n = 0;
  if (!readHeader(major, n))
    return *this;
  if (major != 2 || n > cap || pos + n > len)
  {
    _last_error = 1;
    return *this;
  }
  if (n > 0)
    memcpy(out, buf + pos, n);
  pos += n;
  outLen = n;
  return *this;
}

CborDecoder &CborDecoder::read(std::vector<uint8_t> &out)
{
  if (_last_error != 0)
    return *this;
  uint8_t major = 0;
  uint32_t n = 0;
  if (!readHeader(major, n))
    return *this;
  if (major != 2 || pos + n > len)
  {
    _last_error = 1;
    return *this;
  }
  out.assign(buf + pos, buf + pos + n);
  pos += n;
  return *this;
}

CborDecoder &CborDecoder::read(std::string &out)
{
  if (_last_error != 0)
    return *this;
  uint8_t major = 0;
  uint32_t n = 0;
  if (!readHeader(major, n))
    return *this;
  if (major != 3 || pos + n > len)
  {
    _last_error = 1;
    return *this;
  }
  out.assign((const char *)(buf + pos), n);
  pos += n;
  return *this;
}

CborDecoder &CborDecoder::read(std::vector<uint32_t> &out)
{
  if (_last_error != 0)
    return *this;
  uint8_t major = 0;
  uint32_t n = 0;
  if (!readHeader(major, n))
    return *this;
  if (major != 4)
  {
    _last_error = 1;
    return *this;
  }
  out.clear();
  out.reserve(n);
  for (uint32_t i = 0; i < n; ++i)
  {
    uint32_t v = 0;
    read(v);
    if (_last_error != 0)
      return *this;
    out.push_back(v);
  }
  return *this;
}

bool CborDecoder::readHeader(uint8_t &major, uint32_t &value)
{
  if (_last_error != 0)
    return false;
  if (!cborReadHeader(buf, len, pos, major, value))
  {
    _last_error = 1;
    return false;
  }
  return true;
}
