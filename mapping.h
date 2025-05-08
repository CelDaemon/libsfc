#pragma once

#include <assert.h>

#include "sfc.h"
#include "header.h"
auto constexpr copier_size = 512;

inline static size_t raw_header_offset(const enum sfc_mapping mapping) {
    switch (mapping) {
        case SFC_MAP_LO:
            return 0x7FC0;
        case SFC_MAP_HI:
            return 0xFFC0;
        case SFC_MAP_EX_HI:
            return 0x40FFC0;
        default:
            unreachable();
    }
}

inline static size_t header_offset(const enum sfc_mapping mapping, const bool copier) {
    auto const offset = raw_header_offset(mapping);
    return copier ? offset + copier_size : offset;
}

inline static struct sfc_header *rom_header(const struct sfc_rom *rom) {
    auto const offset = header_offset(rom->mapping, rom->copier);
    assert(rom->size > offset + header_size);
    return rom->data + offset;
}