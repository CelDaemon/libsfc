#include "last_bit.h"
#include "mapping.h"

#include <stdint.h>


static size_t pow2(const size_t n) {
    return ((size_t) 1) << n;
}

uint16_t sfc_checksum(const struct sfc_rom *rom) {
    size_t const offset = sfc_data_offset(rom->copier);

    size_t const size = rom->size - offset;
    const uint8_t * data = ((uint8_t*)rom->data) + offset;

    size_t const main_size = pow2(last_bit(size));
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
