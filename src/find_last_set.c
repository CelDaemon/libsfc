/*
    SPDX-FileCopyrightText: 2025 CelDaemon <celdaemon@voidgroup.net>
    
    SPDX-License-Identifier: BSD-3-Clause
*/

#include "find_last_set.h"

#include <assert.h>
#include <limits.h>

#if _MSC_VER
#include <intrin.h>
#endif

#if defined(__has_builtin)
#define HAS_BUILTIN(x) __has_builtin(x)
#else
#define HAS_BUILTIN(x) 0
#endif

uint32_t find_last_set(uint32_t const x)
{
    assert(x != 0);
#if HAS_BUILTIN(__builtin_clzl)
    return sizeof(uint32_t) * CHAR_BIT - __builtin_clz(x) - 1;
#elif defined(_MSC_VER)
    unsigned long index;
    _BitScanReverse(&index, x);
    return sizeof(uint32_t) * CHAR_BIT - index - 1;
#else
    uint32_t a = x;
    uint32_t c = 0;
    while (a >>= 1)
        c++;
    return c;
#endif
}
