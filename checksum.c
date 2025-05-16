#include <strings.h>

#include "mapping.h"

#include "sfc.h"


static size_t log2z(const size_t x) {
    return ffsl((long) x);
}
static size_t pow2z(const size_t n) {
    return 1 << n;
}

uint16_t sfc_checksum(const struct sfc_rom *rom) {
    auto const offset = sfc_data_offset(rom->copier);
    auto const size = rom->size - offset;
    const uint8_t * data = rom->data + offset;
    auto const main_size = pow2z(log2z(size));
    auto const remainder_size = size - main_size;
    uint16_t main_accumulator = 0;
    for (auto i = 0; i < main_size; i++) {
        main_accumulator += data[i];
    }
    uint16_t remainder_accumulator = 0;
    if (remainder_size != 0) {
        for (auto i = 0; i < main_size; i++) {
            remainder_accumulator += data[(i % remainder_size) + main_size];
        }
    }
    return main_accumulator + remainder_accumulator;
}