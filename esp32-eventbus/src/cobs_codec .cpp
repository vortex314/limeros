#include "cobs_codec.h"

size_t cobs_encode(const uint8_t *data,
                  size_t len,
                  uint8_t *out,
                  size_t outCap)
{
  if (outCap == 0)
  {
    return 0;
  }

  size_t readIdx = 0;
  size_t writeIdx = 1;
  size_t codeIdx = 0;
  uint8_t code = 1;

  while (readIdx < len)
  {
    if (data[readIdx] == 0x00)
    {
      if (codeIdx >= outCap || writeIdx >= outCap)
      {
        return 0;
      }

      out[codeIdx] = code;
      codeIdx = writeIdx++;
      code = 1;
      ++readIdx;
    }
    else
    {
      if (writeIdx >= outCap)
      {
        return 0;
      }

      out[writeIdx++] = data[readIdx++];
      ++code;

      if (code == 0xFF)
      {
        if (codeIdx >= outCap || writeIdx >= outCap)
        {
          return 0;
        }

        out[codeIdx] = code;
        codeIdx = writeIdx++;
        code = 1;
      }
    }
  }

  if (codeIdx >= outCap)
  {
    return 0;
  }

  out[codeIdx] = code;
  return writeIdx;
}

int cobs_decode(const uint8_t *frame,
               size_t len,
               uint8_t *out,
               size_t outCap)
{
  size_t readIdx = 0;
  size_t writeIdx = 0;

  while (readIdx < len)
  {
    uint8_t code = frame[readIdx++];

    if (code == 0x00)
    {
      return -1;
    }

    size_t count = code - 1;

    if (readIdx + count > len)
    {
      return -1;
    }

    while (count-- > 0)
    {
      if (writeIdx >= outCap)
      {
        return -1;
      }

      out[writeIdx++] = frame[readIdx++];
    }

    if (code != 0xFF && readIdx < len)
    {
      if (writeIdx >= outCap)
      {
        return -1;
      }

      out[writeIdx++] = 0x00;
    }
  }

  return (int)writeIdx;
}
