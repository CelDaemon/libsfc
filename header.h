#pragma once
#include <stdint.h>

#include "sfc.h"

struct sfc_header {
    char title[max_title_length];
    uint8_t rom_mode;
    uint8_t chipset;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t country;
    uint8_t developer;
    uint8_t version;
    uint16_t checksum_complement;
    uint16_t checksum;
};



auto constexpr header_size = sizeof(struct sfc_header);