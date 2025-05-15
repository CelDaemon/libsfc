#include "mapping.h"
#include "sfc.h"


uint16_t sfc_checksum(const struct sfc_rom *rom) {
    auto const offset = data_offset(rom->copier);
    auto const size = rom->size - offset;
    const uint8_t * data = rom->data + offset;
    uint16_t accumulator = 0;
    for (auto i = 0; i < size; i++) {
        accumulator += data[i];
    }
    return accumulator;
}