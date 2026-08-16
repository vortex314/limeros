#ifndef CBOR_CODEC_H
#define CBOR_CODEC_H
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "option.h"
#include <string>
#include <type_traits>
#include <vector>

#include "buffer.h"

// ---------------------------------------------------------------------------
// CborEncoder - fluent CBOR writer.
//
// Every mutating call returns *this so calls can be chained, and each method
// is a no-op once an error has occurred (_last_error != 0). Check ok()/error()
// after the chain.
// ---------------------------------------------------------------------------
class CborEncoder
{
private:
  Buffer& _buffer;
  int _last_error;

  bool putByte(uint8_t b);
  bool putBytes(const uint8_t *p, size_t n);
  bool putU16(uint16_t v);
  bool putU32(uint32_t v);
  bool putFloat32(float v);
  bool writeHead(uint8_t major, uint32_t value);

public:
  CborEncoder(Buffer &buffer);

  bool ok() const;
  int error() const;
  size_t size() const;
  const uint8_t *data() const;
  int err() const { return _last_error; }

  // Containers.
  CborEncoder &beginMap(uint32_t count);
  CborEncoder &beginArray(uint32_t count);

  // Explicit writers.
  CborEncoder &write(uint32_t value);
  CborEncoder &write(int32_t value);
  CborEncoder &write(uint64_t value);
  CborEncoder &write(float value);
  CborEncoder &write(bool value);
  CborEncoder &write(const char *text);
  CborEncoder &write(const std::string &text);
  CborEncoder &writeNull();
  CborEncoder &write(const uint8_t *p, size_t n);
  CborEncoder &write(const std::vector<uint8_t> &v);
  CborEncoder &write(const std::vector<uint32_t> &v);

  // Writes the value if present, otherwise a CBOR null.
  template <typename T>
  CborEncoder &write(const Option<T> &value)
  {
    if (_last_error != 0)
      return *this;
    if (!value)
      return writeNull();
    return write(*value);
  }

};

// ---------------------------------------------------------------------------
// CborDecoder - fluent CBOR reader.
//
// Same fluent / error-latched style as CborEncoder: every read returns *this
// and becomes a no-op once an error has occurred.
// ---------------------------------------------------------------------------
class CborDecoder
{
public:
  explicit CborDecoder(Buffer& buffer) : buf(buffer), _last_error(0) {};

  bool ok() const;
  int error() const;
  size_t offset() const;
  int err() const { return _last_error; }

  // Containers. count is the expected number of elements; it is validated
  // against the count actually encoded in the CBOR stream.
  CborDecoder &beginArray(uint32_t count);
  CborDecoder &beginMap(uint32_t count);

  // Scalar readers.
  CborDecoder &read(uint32_t &out);
  CborDecoder &read(int32_t &out);
  CborDecoder &read(uint64_t &out);
  CborDecoder &read(float &out);
  CborDecoder &read(bool &out);
  CborDecoder &read(char *out, size_t maxLen);
  CborDecoder &read(std::string &out);
  CborDecoder &readNull();
  CborDecoder &read(uint8_t *out, size_t cap, size_t &outLen);
  CborDecoder &read(std::vector<uint8_t> &out);
  CborDecoder &read(std::vector<uint32_t> &out);

  // Reads the value if present, otherwise consumes a null and leaves out unset.
  template <typename T>
  CborDecoder &read(Option<T> &out)
  {
    if (_last_error != 0)
      return *this;
    if (pos >= buf.size())
    {
      _last_error = 1; // no data left in buffer
      return *this;
    }
    if (buf.data()[pos] == 0xF6) // null
    {
      ++pos;
      out.reset();
      return *this;
    }
    T v{};
    read(v);
    if (_last_error == 0)
      out = v;
    return *this;
  }

private:
  bool readHeader(uint8_t &major, uint32_t &value);
  size_t pos;
  Buffer& buf;
  int _last_error;
};

bool cborFindBool(const uint8_t *data,
                  size_t len,
                  const char *key,
                  bool &out);

#endif // CBOR_CODEC_H