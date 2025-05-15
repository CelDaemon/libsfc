#include "mapping.h"

size_t header_offset(const enum sfc_map mapping) {
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

size_t data_offset(const bool copier) {
    return copier ? copier_size : 0;
}