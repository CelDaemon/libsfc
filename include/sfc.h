#ifndef _SFC_H
#define _SFC_H

typedef int sfc_bool;
#define SFC_TRUE 1
#define SFC_FALSE 0

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

struct sfc_rom *sfc_create_rom(const void *data, size_t size, const sfc_bool *copier, const enum sfc_map *map);
void sfc_destroy_rom(struct sfc_rom *rom);

#endif
