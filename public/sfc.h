#pragma once
#include <stdint.h>
#include <stddef.h>

enum sfc_mapping {
    SFC_MAP_SMART, // Unspecified, requires inferring the mapping
    SFC_MAP_LO,
    SFC_MAP_HI,
    SFC_MAP_EX_HI,
    SFC_MAP_INVALID = -1,
    SFC_MAP__CONCRETE_FIRST = SFC_MAP_LO,
    SFC_MAP__CONCRETE_LAST = SFC_MAP_EX_HI,
};

#define SFC_MAP_CONCRETE(mapping) mapping >= SFC_MAP__CONCRETE_FIRST && mapping <= SFC_MAP__CONCRETE_LAST

struct sfc_rom {
    uint8_t *data;
    size_t size;
    enum sfc_mapping mapping;
    bool copier;
};




bool sfc_load_rom(const char *path, enum sfc_mapping mapping, bool copier, struct sfc_rom *rom);
void sfc_unload_rom(struct sfc_rom *rom);