/*
    SPDX-FileCopyrightText: 2025 CelDaemon <celdaemon@voidgroup.net>

    SPDX-License-Identifier: BSD-3-Clause
*/

#include "header.h"

#include <sfc.h>
#include <stdint.h>
#include <string.h>

#include "offset.h"
#include "find_last_set.h"

#define SFC_HEADER_TITLE_OFFSET 0xC0
#define SFC_HEADER_MAP_MODE_OFFSET 0xD5
#define SFC_HEADER_CARTRIDGE_TYPE_OFFSET 0xD6
#define SFC_HEADER_ROM_SIZE_OFFSET 0xD7
#define SFC_HEADER_RAM_SIZE_OFFSET 0xD8

#define SFC_HEADER_TITLE(x) ((char*) OFFSET_POINTER(x, SFC_HEADER_TITLE_OFFSET))
#define SFC_HEADER_MAP_MODE(x) (*(uint_least8_t*) OFFSET_POINTER(x, SFC_HEADER_MAP_MODE_OFFSET))
#define SFC_HEADER_CARTRIDGE_TYPE(x) (*(uint_least8_t*) OFFSET_POINTER(x, SFC_HEADER_CARTRIDGE_TYPE_OFFSET))
#define SFC_HEADER_ROM_SIZE(x) (*(uint_least8_t*) OFFSET_POINTER(x, SFC_HEADER_ROM_SIZE_OFFSET))
#define SFC_HEADER_RAM_SIZE(x) (*(uint_least8_t*) OFFSET_POINTER(x, SFC_HEADER_RAM_SIZE_OFFSET))

static size_t find_title_size(char const title[SFC_HEADER_TITLE_MAX_SIZE + 1])
{
    for (size_t i = 0; i < SFC_HEADER_TITLE_MAX_SIZE; i++)
    {
        if (title[SFC_HEADER_TITLE_MAX_SIZE - i] != ' ')
            return SFC_HEADER_TITLE_MAX_SIZE + 1 - i;
    }
    return 0;
}

char *sfc_header_title(sfc_header const header, char title[SFC_HEADER_TITLE_MAX_SIZE + 1])
{
    assert(header != NULL);
    assert(title != NULL);
    size_t const size = find_title_size(SFC_HEADER_TITLE(header));
    memcpy(title, SFC_HEADER_TITLE(header), size);
    title[size] = '\0';
    return title;
}

bool sfc_header_set_title(sfc_header const header, char title[])
{
    assert(header != NULL);
    assert(title != NULL);
    size_t const size = strlen(title);
    if (size > SFC_HEADER_TITLE_MAX_SIZE)
        return false;
    for (size_t i = 0; i < size; i++)
    {
        if (!(title[i] == ' ' || (title[i] >= 'A' && title[i] <= 'Z')))
            return false;
    }
    memcpy(SFC_HEADER_TITLE(header), title, size);
    memset(SFC_HEADER_TITLE(header) + size, ' ', SFC_HEADER_TITLE_MAX_SIZE - size);
    return true;
}

enum sfc_speed sfc_header_speed(sfc_header const header)
{
    assert(header != NULL);
    return (SFC_HEADER_MAP_MODE(header) & 0x10) > 0 ? SFC_FAST : SFC_SLOW;
}

void sfc_header_set_speed(sfc_header const header, enum sfc_speed const speed)
{
    assert(header != NULL);
    uint_least8_t const value = speed == SFC_FAST ? 1 : 0;
    uint_least8_t mode = SFC_HEADER_MAP_MODE(header);
    mode &= ~0x10;
    mode |= value << 4;
    SFC_HEADER_MAP_MODE(header) = mode;
}

bool sfc_header_map(sfc_header const header, enum sfc_map * const map)
{
    assert(header != NULL);
    switch (SFC_HEADER_MAP_MODE(header) & 0xF)
    {
    case 0:
        *map = SFC_MAP_LO;
        return true;
    case 1:
        *map = SFC_MAP_HI;
        return true;
    case 5:
        *map = SFC_MAP_EX_HI;
        return true;
    default:
        return false;
    }
}

bool sfc_header_set_map(sfc_header const header, enum sfc_map const map)
{
    assert(header != NULL);
    uint_least8_t mode = SFC_HEADER_MAP_MODE(header);
    mode &= ~0xF;
    switch (map)
    {
    case SFC_MAP_LO:
        mode |= 0;
        break;
    case SFC_MAP_HI:
        mode |= 1;
        break;
    case SFC_MAP_EX_HI:
        mode |= 5;
        break;
    default:
        return false;
    }
    SFC_HEADER_MAP_MODE(header) = mode;
    return true;
}

bool sfc_header_cartridge_type(sfc_header const header, struct sfc_cartridge_type * const cartridge_type)
{
    struct sfc_cartridge_type output = {
        false,
        false,
        false
    };
    switch (SFC_HEADER_CARTRIDGE_TYPE(header) & 0xF)
    {
    case 0:
        break;
    case 1:
        output.ram = true;
        break;
    case 2:
        output.ram = true;
        output.battery = true;
        break;
    case 3:
        output.coprocessor = true;
        break;
    case 4:
        output.ram = true;
        output.coprocessor = true;
        break;
    case 5:
        output.ram = true;
        output.battery = true;
        output.coprocessor = true;
        break;
    case 6:
        output.battery = true;
        output.coprocessor = true;
        break;
    default:
        return false;
    }
    *cartridge_type = output;
    return true;
}


static int_least8_t cartridge_type_id(struct sfc_cartridge_type const cartridge_type)
{
    if (!cartridge_type.coprocessor)
    {
        if (!cartridge_type.ram)
        {
            if (cartridge_type.battery)
                return -1;
            return 0;
        }
        if (!cartridge_type.battery)
            return 1;
        return 2;
    }
    if (cartridge_type.ram)
    {
        if (!cartridge_type.battery)
            return 4;
        return 5;
    }
    if (!cartridge_type.battery)
        return 3;
    return 6;
}

bool sfc_header_set_cartridge_type(sfc_header const header, struct sfc_cartridge_type const cartridge_type)
{
    int_least8_t const new_id = cartridge_type_id(cartridge_type);
    if (new_id == -1)
        return false;
    uint_least8_t id = SFC_HEADER_CARTRIDGE_TYPE(header);
    id &= ~0xF;
    id |= (new_id & 0xF);
    SFC_HEADER_CARTRIDGE_TYPE(header) = id;
    return true;
}

uint_least32_t sfc_header_rom_size(sfc_header const header)
{
    return 1 << SFC_HEADER_ROM_SIZE(header);
}

bool sfc_header_set_rom_size(sfc_header const header, uint_least32_t const size)
{
    uint_least32_t const value = find_last_set(size);
    if ((size & ~(1 << value)) != 0)
        return false;
    SFC_HEADER_ROM_SIZE(header) = (uint_least8_t) value & 0xFF;
    return true;
}

uint_least32_t sfc_header_ram_size(sfc_header const header)
{
    return 1 << SFC_HEADER_RAM_SIZE(header);
}

bool sfc_header_set_ram_size(sfc_header const header, uint_least32_t const size)
{
    size_t const value = find_last_set(size);
    if ((size & ~(1 << value)) != 0)
        return false;
    SFC_HEADER_RAM_SIZE(header) = (uint_least8_t) value & 0xFF;
    return true;
}
