#include <assert.h>
#include <limits.h>

#include "mapping.h"


static size_t log2z(const size_t x) {
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

static size_t pow2(const size_t n) {
    return 1 << n;
}

uint16_t sfc_checksum(const struct sfc_rom *rom) {
    size_t const offset = sfc_data_offset(rom->copier);

    size_t const size = rom->size - offset;
    const uint8_t * data = ((uint8_t*)rom->data) + offset;

    size_t const main_size = pow2(log2z(size));
    size_t const remainder_size = size - main_size;

    uint16_t main_accumulator = 0;
    for (size_t i = 0; i < main_size; i++) {
        main_accumulator += data[i];
    }

    uint16_t remainder_accumulator = 0;
    if (remainder_size != 0) {
        for (size_t i = 0; i < main_size; i++) {
            remainder_accumulator += data[(i % remainder_size) + main_size];
        }
    }

    return main_accumulator + remainder_accumulator;
}