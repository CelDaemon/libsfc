#pragma once
#include "sfc.h"

auto constexpr copier_size = 512;

static size_t raw_header_offset(const enum sfc_mapping mapping) {
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

static size_t header_offset(const enum sfc_mapping mapping, const bool copier) {
    auto const offset = raw_header_offset(mapping);
    return copier ? offset + copier_size : offset;
}