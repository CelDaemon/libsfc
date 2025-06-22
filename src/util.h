/*
    SPDX-FileCopyrightText: 2025 CelDaemon <celdaemon@voidgroup.net>

    SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _SFC_UTIL_H
#define _SFC_UTIL_H

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

#define OFFSET_POINTER(ptr, offset) ((void*) (((char*)(ptr)) + (offset)))

static size_t msb(size_t const x)
{
    assert(x != 0);
#if HAS_BUILTIN(__builtin_clzl)
    return sizeof(size_t) * CHAR_BIT - __builtin_clzl(x) - 1;
#elif defined(_MSC_VER)
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

#endif
