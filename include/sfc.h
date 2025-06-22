/*
    SPDX-FileCopyrightText: 2025 CelDaemon <celdaemon@voidgroup.net>

    SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _SFC_H
#define _SFC_H

#include <stdbool.h>
#include <stddef.h>

#define SFC_HEADER_TITLE_MAX_SIZE 21

enum sfc_map
{
    SFC_MAP_LO,
    SFC_MAP_HI,
    SFC_MAP_EX_HI
};

enum sfc_speed
{
    SFC_SLOW,
    SFC_FAST
};

struct sfc_chipset
{
    bool ram:1;
    bool battery:1;
    bool coprocessor:1;
};

typedef void *sfc_header;

struct sfc_rom
{
    void *data;
    size_t size;
    sfc_header header;
};


struct sfc_rom *sfc_create_rom(void const *input_data, size_t size, bool const *copier, enum sfc_map const *map);
void sfc_destroy_rom(struct sfc_rom *rom);
sfc_header sfc_rom_header(struct sfc_rom const * rom);

char *sfc_header_title(sfc_header header, char title[SFC_HEADER_TITLE_MAX_SIZE + 1]);
bool sfc_header_set_title(sfc_header header, char title[]);

enum sfc_speed sfc_header_speed(sfc_header header);
void sfc_header_set_speed(sfc_header header, enum sfc_speed speed);

bool sfc_header_map(sfc_header header, enum sfc_map * map);
bool sfc_header_set_map(sfc_header header, enum sfc_map map);

bool sfc_header_chipset(sfc_header header, struct sfc_chipset * chipset);
bool sfc_header_set_chipset(sfc_header header, struct sfc_chipset chipset);

size_t sfc_header_rom_size(sfc_header header);
bool sfc_header_set_rom_size(sfc_header header, size_t size);

size_t sfc_header_ram_size(sfc_header header);
bool sfc_header_set_ram_size(sfc_header header, size_t size);

#endif
