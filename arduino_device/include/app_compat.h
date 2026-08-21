#ifndef APP_COMPAT_H
#define APP_COMPAT_H

// -------------------------------------------------------------
// 1. C Standard Headers (Provides size_t, va_list, etc.)
// -------------------------------------------------------------
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>

// Undefine newlib-nano macro aliases so real C prototypes are created
#undef printf
#undef sprintf
#undef snprintf
#undef vsnprintf
#undef vprintf

// -------------------------------------------------------------
// 2. Global C Symbol Declarations
// -------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

// Concrete declarations required by GCC libstdc++ <cstdio> using ::printf;
int printf(const char *format, ...);
int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);
int vprintf(const char *format, va_list ap);

// Non-standard string conversion prototypes
char* itoa(int value, char *string, int radix);
char* ltoa(long value, char *string, int radix);
char* ultoa(unsigned long value, char *string, int radix);

#ifdef __cplusplus
}
#endif

// -------------------------------------------------------------
// 3. C++ Standard Library Headers
// -------------------------------------------------------------
#include <utility>
#include <type_traits>
#include <algorithm>
#include <iterator>
#include <limits>
#include <memory>
#include <array>
#include <vector>
#include <string>
#include <unordered_map>
#include <atomic>
#include <functional>

// -------------------------------------------------------------
// 4. Arduino / Energia Core Headers
// -------------------------------------------------------------
#include <Arduino.h>

// -------------------------------------------------------------
// 5. Sanitize Arduino Macro Pollution
// -------------------------------------------------------------
#undef printf
#undef sprintf
#undef snprintf
#undef vsnprintf
#undef min
#undef max
#undef abs
#undef round
#undef swap

namespace compat {
    template <typename T>
    constexpr const T& min(const T& a, const T& b) {
        return (b < a) ? b : a;
    }

    template <typename T>
    constexpr const T& max(const T& a, const T& b) {
        return (a < b) ? b : a;
    }
}

#endif // APP_COMPAT_H