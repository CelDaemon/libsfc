/*
    SPDX-FileCopyrightText: 2025 CelDaemon <celdaemon@voidgroup.net>

    SPDX-License-Identifier: BSD-3-Clause
*/

#include "header.h"

#include <assert.h>
#include <sfc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define SFC_HEADER_TITLE_OFFSET 0xC0
#define SFC_HEADER_ROM_MODE_OFFSET 0xD5
#define SFC_HEADER_CHIPSET_OFFSET 0xD6

#define SFC_HEADER_TITLE(x) ((char*) OFFSET_POINTER(x, SFC_HEADER_TITLE_OFFSET))
#define SFC_HEADER_ROM_MODE(x) (*(uint_least8_t*) OFFSET_POINTER(x, SFC_HEADER_ROM_MODE_OFFSET))
#define SFC_HEADER_CHIPSET(x) (*(uint_least8_t*) OFFSET_POINTER(x, SFC_HEADER_CHIPSET_OFFSET))

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

void sfc_header_set_title(sfc_header const header, char title[])
{
    assert(header != NULL);
    assert(title != NULL);
    size_t const size = strlen(title);
    assert(size <= SFC_HEADER_TITLE_MAX_SIZE);
    memcpy(SFC_HEADER_TITLE(header), title, size);
    memset(SFC_HEADER_TITLE(header) + size, ' ', SFC_HEADER_TITLE_MAX_SIZE - size);
}

enum sfc_speed sfc_header_speed(sfc_header const header)
{
    assert(header != NULL);
    return (SFC_HEADER_ROM_MODE(header) & 0x10) > 0 ? SFC_FAST : SFC_SLOW;
}

void sfc_header_set_speed(sfc_header const header, enum sfc_speed const speed)
{
    assert(header != NULL);
    uint_least8_t const value = speed == SFC_FAST ? 1 : 0;
    uint_least8_t mode = SFC_HEADER_ROM_MODE(header);
    mode &= ~0x10;
    mode |= value << 4;
    SFC_HEADER_ROM_MODE(header) = mode;
}

enum sfc_map sfc_header_map(sfc_header const header)
{
    assert(header != NULL);
    switch (SFC_HEADER_ROM_MODE(header) & 0xF)
    {
    case 0:
        return SFC_MAP_LO;
    case 1:
        return SFC_MAP_HI;
    case 5:
        return SFC_MAP_EX_HI;
    default:
        abort();
    }
}

void sfc_header_set_map(sfc_header const header, enum sfc_map const map)
{
    assert(header != NULL);
    uint_least8_t mode = SFC_HEADER_ROM_MODE(header);
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
        abort();
    }
    SFC_HEADER_ROM_MODE(header) = mode;
}

struct sfc_chipset sfc_header_chipset(sfc_header const header)
{
    struct sfc_chipset output = {
        false,
        false,
        false
    };
    switch (SFC_HEADER_CHIPSET(header) & 0xF)
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
        abort();
    }
    return output;
}


static int_least8_t chipset_id(struct sfc_chipset const chipset)
{
    if (!chipset.coprocessor)
    {
        if (!chipset.ram)
        {
            if (chipset.battery)
                return -1;
            return 0;
        }
        if (!chipset.battery)
            return 1;
        return 2;
    }
    if (chipset.ram)
    {
        if (!chipset.battery)
            return 4;
        return 5;
    }
    if (!chipset.battery)
        return 3;
    return 6;
}

bool sfc_header_set_chipset(sfc_header const header, struct sfc_chipset const chipset)
{
    int_least8_t const new_id = chipset_id(chipset);
    if (new_id == -1)
        return false;
    uint_least8_t id = SFC_HEADER_CHIPSET(header);
    id &= ~0xF;
    id |= (new_id & 0xF);
    SFC_HEADER_CHIPSET(header) = id;
    return true;
}
