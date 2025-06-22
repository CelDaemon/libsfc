/*
    SPDX-FileCopyrightText: 2025 celdaemon <celdaemon@voidgroup.net>
    
    SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _SFC_FLS_H
#define _SFC_FLS_H

#include <assert.h>
#include <limits.h>
#include <stddef.h>

#if _MSC_VER
#include <intrin.h>
#endif

#if defined(__has_builtin)
#define HAS_BUILTIN(x) __has_builtin(x)
#else
#define HAS_BUILTIN(x) 0
#endif

static uint_least32_t find_last_set(uint_least32_t const x)
{
    assert(x != 0);
#if HAS_BUILTIN(__builtin_clzl)
    return sizeof(uint_least32_t) * CHAR_BIT - __builtin_clz(x) - 1;
#elif defined(_MSC_VER)
    uint_least32_t index;
    _BitScanReverse(&index, x);
    return sizeof(uint_least32_t) * CHAR_BIT - index - 1;
#else
    uint_least32_t a = x;
    uint_least32_t c = 0;
    while (a >>= 1)
        c++;
    return c;
#endif
}

#endif
