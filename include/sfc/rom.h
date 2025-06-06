#pragma once
#include <stdbool.h>

enum sfc_copier
{
    SFC_CPY_SMART,
    SFC_CPY_FALSE,
    SFC_CPY_TRUE
};

#define SFC_CPY_CONCRETE(copier) ((copier) == SFC_CPY_FALSE || (copier) == SFC_CPY_TRUE)

enum sfc_mapping {
    SFC_MAP_SMART, // Unspecified, requires inferring the mapping
    SFC_MAP_LO,
    SFC_MAP_HI,
    SFC_MAP_EX_HI,
    SFC_MAP_INVALID = -1,
    SFC_MAP__CONCRETE_FIRST = SFC_MAP_LO,
    SFC_MAP__CONCRETE_LAST = SFC_MAP_EX_HI,
    SFC_MAP__FIRST = SFC_MAP_SMART,
    SFC_MAP__LAST = SFC_MAP_EX_HI
};

#define SFC_MAP_CONCRETE(mapping) ((mapping) >= SFC_MAP__CONCRETE_FIRST && (mapping) <= SFC_MAP__CONCRETE_LAST)

struct sfc_rom {
    void *data;
    size_t size;
    enum sfc_mapping map;
    bool copier;
};

void *sfc_load_file(const char *path, size_t *size);
bool sfc_load_rom(void *data, size_t size, enum sfc_mapping map, enum sfc_copier copier, struct sfc_rom *rom);
void sfc_unload_rom(struct sfc_rom *rom);
bool sfc_save_rom(const struct sfc_rom *rom, const char *path);
