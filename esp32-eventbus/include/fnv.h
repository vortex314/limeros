#ifndef FNV_H
#define FNV_H

#include <cstddef>
#include <cstdint>

namespace fnv
{
constexpr uint32_t FNV1A_32_OFFSET_BASIS = 2166136261u;
constexpr uint32_t FNV1A_32_PRIME = 16777619u;

// Hash a null-terminated C string.
constexpr uint32_t fnv1a_32(const char *str,
                            uint32_t hash = FNV1A_32_OFFSET_BASIS)
{
    return (*str == '\0')
               ? hash
               : fnv1a_32(str + 1,
                          (hash ^ static_cast<uint32_t>(
                                      static_cast<unsigned char>(*str))) *
                              FNV1A_32_PRIME);
}

// Hash a buffer of known length (works with embedded null bytes).
constexpr uint32_t fnv1a_32_n(const char *data, size_t len,
                              uint32_t hash = FNV1A_32_OFFSET_BASIS)
{
    return (len == 0)
               ? hash
               : fnv1a_32_n(data + 1, len - 1,
                            (hash ^ static_cast<uint32_t>(
                                        static_cast<unsigned char>(*data))) *
                                FNV1A_32_PRIME);
}

// Convenience helper for string literals.
template <size_t N>
constexpr uint32_t fnv1a_32(const char (&str)[N])
{
    return fnv1a_32_n(str, N - 1);
}

} // namespace fnv

#endif // FNV_H
