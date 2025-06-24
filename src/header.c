/*
    Copyright 2025 CelDaemon

    Redistribution and use in source and binary forms, with or without modification,are permitted provided
    that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
    the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or
    promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
    IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "header.h"

#include <sfc.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "offset.h"
#include "find_last_set.h"

#define SFC_HEADER_TITLE_OFFSET 0xC0
#define SFC_HEADER_MAP_MODE_OFFSET 0xD5
#define SFC_HEADER_CARTRIDGE_TYPE_OFFSET 0xD6
#define SFC_HEADER_ROM_SIZE_OFFSET 0xD7
#define SFC_HEADER_RAM_SIZE_OFFSET 0xD8
#define SFC_HEADER_DESTINATION_CODE_OFFSET 0xD9
#define SFC_HEADER_DEVELOPER_ID_OFFSET 0xDA
#define SFC_HEADER_VERSION_OFFSET 0xDB

#define SFC_HEADER_TITLE(x) ((char*) OFFSET_POINTER(x, SFC_HEADER_TITLE_OFFSET))
#define SFC_HEADER_MAP_MODE(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_MAP_MODE_OFFSET))
#define SFC_HEADER_CARTRIDGE_TYPE(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_CARTRIDGE_TYPE_OFFSET))
#define SFC_HEADER_ROM_SIZE(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_ROM_SIZE_OFFSET))
#define SFC_HEADER_RAM_SIZE(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_RAM_SIZE_OFFSET))
#define SFC_HEADER_DESTINATION_CODE(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_DESTINATION_CODE_OFFSET))
#define SFC_HEADER_DEVELOPER_ID(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_DEVELOPER_ID_OFFSET))
#define SFC_HEADER_VERSION(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_VERSION_OFFSET))


static size_t find_title_size(char const title[SFC_HEADER_TITLE_MAX_SIZE + 1])
{
    for (size_t i = 0; i < SFC_HEADER_TITLE_MAX_SIZE; i++)
    {
        size_t const length = SFC_HEADER_TITLE_MAX_SIZE - i;
        if (title[length - 1] != ' ')
            return length;
    }
    return 0;
}

char *sfc_header_title(sfc_header const * const header, char title[SFC_HEADER_TITLE_MAX_SIZE + 1])
{
    assert(header != NULL);
    assert(title != NULL);
    size_t const size = find_title_size(SFC_HEADER_TITLE(header));
    memcpy(title, SFC_HEADER_TITLE(header), size);
    title[size] = '\0';
    return title;
}

bool sfc_header_set_title(sfc_header * const header, char title[])
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

enum sfc_speed sfc_header_speed(sfc_header const * const header)
{
    assert(header != NULL);
    return (SFC_HEADER_MAP_MODE(header) & 0x10) > 0 ? SFC_FAST : SFC_SLOW;
}

void sfc_header_set_speed(sfc_header * const header, enum sfc_speed const speed)
{
    assert(header != NULL);
    uint8_t const value = speed == SFC_FAST ? 1 : 0;
    uint8_t mode = SFC_HEADER_MAP_MODE(header);
    mode &= ~0x10;
    mode |= value << 4;
    SFC_HEADER_MAP_MODE(header) = mode;
}

bool sfc_header_map(sfc_header const * const header, enum sfc_map * const map)
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

bool sfc_header_set_map(sfc_header * const header, enum sfc_map const map)
{
    assert(header != NULL);
    uint8_t mode = SFC_HEADER_MAP_MODE(header);
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

bool sfc_header_cartridge_type(sfc_header const * const header, struct sfc_cartridge_type * const cartridge_type)
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

bool sfc_header_set_cartridge_type(sfc_header * const header, struct sfc_cartridge_type const cartridge_type)
{
    int8_t const new_id = cartridge_type_id(cartridge_type);
    if (new_id == -1)
        return false;
    uint8_t id = SFC_HEADER_CARTRIDGE_TYPE(header);
    id &= ~0xF;
    id |= (new_id & 0xF);
    SFC_HEADER_CARTRIDGE_TYPE(header) = id;
    return true;
}

uint32_t sfc_header_rom_size(sfc_header const * const header)
{
    return 1 << SFC_HEADER_ROM_SIZE(header);
}

bool sfc_header_set_rom_size(sfc_header * const header, uint32_t const size)
{
    uint32_t const value = find_last_set(size);
    if ((size & ~(1 << value)) != 0)
        return false;
    SFC_HEADER_ROM_SIZE(header) = (uint8_t) value;
    return true;
}

uint32_t sfc_header_ram_size(sfc_header const * const header)
{
    return 1 << SFC_HEADER_RAM_SIZE(header);
}

bool sfc_header_set_ram_size(sfc_header * const header, uint32_t const size)
{
    size_t const value = find_last_set(size);
    if ((size & ~(1 << value)) != 0)
        return false;
    SFC_HEADER_RAM_SIZE(header) = (uint8_t) value;
    return true;
}



bool sfc_header_destination_code(sfc_header const * const header, enum sfc_destination_code * const destination_code)
{
    switch (SFC_HEADER_DESTINATION_CODE(header))
    {
    case 0:
        *destination_code = SFC_DESTINATION_JAPAN;
        return true;
    case 1:
        *destination_code = SFC_DESTINATION_NORTH_AMERICA;
        return true;
    case 2:
        *destination_code = SFC_DESTINATION_EUROPE;
        return true;
    case 3:
        *destination_code = SFC_DESTINATION_SCANDINAVIA;
        return true;
    case 6:
        *destination_code = SFC_DESTINATION_FRENCH_EUROPE;
        return true;
    case 7:
        *destination_code = SFC_DESTINATION_DUTCH;
        return true;
    case 8:
        *destination_code = SFC_DESTINATION_SPANISH;
        return true;
    case 9:
        *destination_code = SFC_DESTINATION_GERMAN;
        return true;
    case 10:
        *destination_code = SFC_DESTINATION_ITALIAN;
        return true;
    case 11:
        *destination_code = SFC_DESTINATION_CHINESE;
        return true;
    case 13:
        *destination_code = SFC_DESTINATION_KOREAN;
        return true;
    case 14:
        *destination_code = SFC_DESTINATION_COMMON;
        return true;
    case 15:
        *destination_code = SFC_DESTINATION_CANADA;
        return true;
    case 16:
        *destination_code = SFC_DESTINATION_BRAZIL;
        return true;
    case 17:
        *destination_code = SFC_DESTINATION_AUSTRALIA;
        return true;
    case 18:
        *destination_code = SFC_DESTINATION_OTHER1;
        return true;
    case 19:
        *destination_code = SFC_DESTINATION_OTHER2;
        return true;
    case 20:
        *destination_code = SFC_DESTINATION_OTHER3;
        return true;
    default:
        return false;
    }
}

bool sfc_header_set_destination_code(sfc_header * const header, enum sfc_destination_code const destination_code)
{
    uint8_t value;
    switch (destination_code)
    {
    case SFC_DESTINATION_JAPAN:
        value = 0;
        break;
    case SFC_DESTINATION_NORTH_AMERICA:
        value = 1;
        break;
    case SFC_DESTINATION_EUROPE:
        value = 2;
        break;
    case SFC_DESTINATION_SCANDINAVIA:
        value = 3;
        break;
    case SFC_DESTINATION_FRENCH_EUROPE:
        value = 6;
        break;
    case SFC_DESTINATION_DUTCH:
        value = 7;
        break;
    case SFC_DESTINATION_SPANISH:
        value = 8;
        break;
    case SFC_DESTINATION_GERMAN:
        value = 9;
        break;
    case SFC_DESTINATION_ITALIAN:
        value = 10;
        break;
    case SFC_DESTINATION_CHINESE:
        value = 11;
        break;
    case SFC_DESTINATION_KOREAN:
        value = 13;
        break;
    case SFC_DESTINATION_COMMON:
        value = 14;
        break;
    case SFC_DESTINATION_CANADA:
        value = 15;
        break;
    case SFC_DESTINATION_BRAZIL:
        value = 16;
        break;
    case SFC_DESTINATION_AUSTRALIA:
        value = 17;
        break;
    case SFC_DESTINATION_OTHER1:
        value = 18;
        break;
    case SFC_DESTINATION_OTHER2:
        value = 19;
        break;
    case SFC_DESTINATION_OTHER3:
        value = 29;
        break;
    default:
        return false;
    }
    SFC_HEADER_DESTINATION_CODE(header) = value;
    return true;
}

bool sfc_header_extended_available(sfc_header const * const header) {
    return sfc_header_developer_id(header) == SFC_HEADER_EXTENDED_AVAILABLE;
}

uint8_t sfc_header_developer_id(sfc_header const * const header) {
    return SFC_HEADER_DEVELOPER_ID(header);
}

void sfc_header_set_developer_id(sfc_header * const header, uint8_t const developer_id) {
    SFC_HEADER_DEVELOPER_ID(header) = developer_id;
}

uint8_t sfc_header_version(sfc_header const * const header) {
    return SFC_HEADER_VERSION(header);
}

void sfc_header_set_version(sfc_header * const header, uint8_t const version) {
    SFC_HEADER_VERSION(header) = version;
}
