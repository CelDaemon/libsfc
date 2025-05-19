#pragma once
#include <stddef.h>
#include <limits.h>
#include <assert.h>
static size_t last_bit(const size_t x) {
    assert(x != 0);
#if defined __has_builtin && __has_builtin(__builtin_clzl)
    return sizeof(size_t) * CHAR_BIT - __builtin_clzl(x) - 1;
#else
    size_t a = x;
    size_t c = 0;
    while (a >>= 1)
        c++;
    return c;
#endif
}