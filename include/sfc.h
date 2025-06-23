/*
    SPDX-FileCopyrightText: 2025 CelDaemon <celdaemon@voidgroup.net>

    SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _SFC_H
#define _SFC_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

struct sfc_cartridge_type
{
    bool ram:1;
    bool battery:1;
    bool coprocessor:1;
};

enum sfc_destination_code
{
    SFC_DESTINATION_JAPAN,
    SFC_DESTINATION_NORTH_AMERICA,
    SFC_DESTINATION_EUROPE,
    SFC_DESTINATION_SCANDINAVIA,
    SFC_DESTINATION_FRENCH_EUROPE,
    SFC_DESTINATION_DUTCH,
    SFC_DESTINATION_SPANISH,
    SFC_DESTINATION_GERMAN,
    SFC_DESTINATION_ITALIAN,
    SFC_DESTINATION_CHINESE,
    SFC_DESTINATION_KOREAN,
    SFC_DESTINATION_COMMON,
    SFC_DESTINATION_CANADA,
    SFC_DESTINATION_BRAZIL,
    SFC_DESTINATION_AUSTRALIA,
    SFC_DESTINATION_OTHER1,
    SFC_DESTINATION_OTHER2,
    SFC_DESTINATION_OTHER3
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

bool sfc_header_cartridge_type(sfc_header header, struct sfc_cartridge_type * cartridge_type);
bool sfc_header_set_cartridge_type(sfc_header header, struct sfc_cartridge_type cartridge_type);

uint32_t sfc_header_rom_size(sfc_header header);
bool sfc_header_set_rom_size(sfc_header header, uint32_t size);

uint32_t sfc_header_ram_size(sfc_header header);
bool sfc_header_set_ram_size(sfc_header header, uint32_t size);

bool sfc_header_destination_code(sfc_header header, enum sfc_destination_code * destination_code);
bool sfc_header_set_destination_code(sfc_header header, enum sfc_destination_code destination_code);

#endif
