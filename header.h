#pragma once
#include <stdint.h>

struct sfc_header {
    char title[21];
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