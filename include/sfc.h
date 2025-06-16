#ifndef _SFC_H
#define _SFC_H

#include <stdbool.h>

enum sfc_map
{
    SFC_MAP_LO,
    SFC_MAP_HI,
    SFC_MAP_EX_HI
};


struct sfc_rom
{
    void *data;
    size_t size;
};

struct sfc_rom *sfc_create_rom(const void *data, size_t size, const bool *copier, const enum sfc_map *map);
void sfc_destroy_rom(struct sfc_rom *rom);

#endif
