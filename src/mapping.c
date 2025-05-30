#include "mapping.h"

#include <stdlib.h>

size_t sfc_header_offset(const enum sfc_mapping mapping) {
    switch (mapping) {
        case SFC_MAP_LO:
            return 0x7FC0;
        case SFC_MAP_HI:
            return 0xFFC0;
        case SFC_MAP_EX_HI:
            return 0x40FFC0;
        default:
            abort();
    }
}

size_t sfc_data_offset(const bool copier) {
    return copier ? 512 : 0;
}
