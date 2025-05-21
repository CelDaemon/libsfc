#pragma once
#include <limits.h>
#include <assert.h>
#include <stddef.h>

#if _MSC_VER
#  include <intrin.h>
#endif

#if defined(__has_builtin)
#  define HAS_BUILTIN(x) __has_builtin(x)
#else
#  define HAS_BUILTIN(x) 0
#endif

static size_t last_bit(const size_t x) {
    assert(x != 0);


#if HAS_BUILTIN(__builtin_clzl)
    return sizeof(size_t) * CHAR_BIT - __builtin_clzl(x) - 1;
#elif defined(_MSC_VERSION)
    size_t index;
    _BitScanReverse(&index, x);
    return sizeof(size_t) * CHAR_BIT - index - 1;
#else
    size_t a = x;
    size_t c = 0;
    while (a >>= 1)
        c++;
    return c;
#endif
}