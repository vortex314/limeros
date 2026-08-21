#ifndef FNV_H
#define FNV_H


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

#endif // FNV_H
