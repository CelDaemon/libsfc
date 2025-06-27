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
#define SFC_HEADER_CHECKSUM_OFFSET 0xDE

#define SFC_HEADER_MAKER_CODE_OFFSET 0xB0
#define SFC_HEADER_GAME_CODE_OFFSET 0xB2
#define SFC_HEADER_EXPANSION_RAM_SIZE_OFFSET 0xBD
#define SFC_HEADER_SPECIAL_VERSION_OFFSET 0xBE
#define SFC_HEADER_CARTRIDGE_SUBTYPE_OFFSET 0xBF



#define SFC_HEADER_TITLE_CONST(x) ((char const*) OFFSET_POINTER_CONST(x, SFC_HEADER_TITLE_OFFSET))
#define SFC_HEADER_TITLE(x) ((char*) OFFSET_POINTER(x, SFC_HEADER_TITLE_OFFSET))

#define SFC_HEADER_MAP_MODE_CONST(x) (*(uint8_t const*) OFFSET_POINTER_CONST(x, SFC_HEADER_MAP_MODE_OFFSET))
#define SFC_HEADER_MAP_MODE(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_MAP_MODE_OFFSET))

#define SFC_HEADER_CARTRIDGE_TYPE_CONST(x) (*(uint8_t const*) OFFSET_POINTER_CONST(x, SFC_HEADER_CARTRIDGE_TYPE_OFFSET))
#define SFC_HEADER_CARTRIDGE_TYPE(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_CARTRIDGE_TYPE_OFFSET))

#define SFC_HEADER_ROM_SIZE_CONST(x) (*(uint8_t const*) OFFSET_POINTER_CONST(x, SFC_HEADER_ROM_SIZE_OFFSET))
#define SFC_HEADER_ROM_SIZE(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_ROM_SIZE_OFFSET))

#define SFC_HEADER_RAM_SIZE_CONST(x) (*(uint8_t const*) OFFSET_POINTER_CONST(x, SFC_HEADER_RAM_SIZE_OFFSET))
#define SFC_HEADER_RAM_SIZE(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_RAM_SIZE_OFFSET))

#define SFC_HEADER_DESTINATION_CODE_CONST(x) (*(uint8_t const*) OFFSET_POINTER_CONST(x, SFC_HEADER_DESTINATION_CODE_OFFSET))
#define SFC_HEADER_DESTINATION_CODE(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_DESTINATION_CODE_OFFSET))

#define SFC_HEADER_DEVELOPER_ID_CONST(x) (*(uint8_t const*) OFFSET_POINTER_CONST(x, SFC_HEADER_DEVELOPER_ID_OFFSET))
#define SFC_HEADER_DEVELOPER_ID(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_DEVELOPER_ID_OFFSET))

#define SFC_HEADER_VERSION_CONST(x) (*(uint8_t const*) OFFSET_POINTER_CONST(x, SFC_HEADER_VERSION_OFFSET))
#define SFC_HEADER_VERSION(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_VERSION_OFFSET))

#define SFC_HEADER_CHECKSUM_CONST(x) (*(uint16_t const*) OFFSET_POINTER_CONST(x, SFC_HEADER_CHECKSUM_OFFSET))
#define SFC_HEADER_CHECKSUM(x) (*(uint16_t*) OFFSET_POINTER(x, SFC_HEADER_CHECKSUM_OFFSET))


#define SFC_HEADER_MAKER_CODE_CONST(x) ((char const*) OFFSET_POINTER_CONST(x, SFC_HEADER_MAKER_CODE_OFFSET))
#define SFC_HEADER_MAKER_CODE(x) ((char*) OFFSET_POINTER(x, SFC_HEADER_MAKER_CODE_OFFSET))

#define SFC_HEADER_GAME_CODE_CONST(x) ((char const*) OFFSET_POINTER_CONST(x, SFC_HEADER_GAME_CODE_OFFSET))
#define SFC_HEADER_GAME_CODE(x) ((char*) OFFSET_POINTER(x, SFC_HEADER_GAME_CODE_OFFSET))

#define SFC_HEADER_EXPANSION_RAM_SIZE_CONST(x) (*(uint8_t const*) OFFSET_POINTER_CONST(x, SFC_HEADER_EXPANSION_RAM_SIZE_OFFSET))
#define SFC_HEADER_EXPANSION_RAM_SIZE(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_EXPANSION_RAM_SIZE_OFFSET))

#define SFC_HEADER_SPECIAL_VERSION_CONST(x) (*(uint8_t const*) OFFSET_POINTER_CONST(x, SFC_HEADER_SPECIAL_VERSION_OFFSET))
#define SFC_HEADER_SPECIAL_VERSION(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_SPECIAL_VERSION_OFFSET))

#define SFC_HEADER_CARTRIDGE_SUBTYPE_CONST(x) (*(uint8_t const*) OFFSET_POINTER_CONST(x, SFC_HEADER_CARTRIDGE_SUBTYPE_OFFSET))
#define SFC_HEADER_CARTRIDGE_SUBTYPE(x) (*(uint8_t*) OFFSET_POINTER(x, SFC_HEADER_CARTRIDGE_SUBTYPE_OFFSET))

#define SFC_EXTENDED_HEADER_AVAILABLE 33


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

bool sfc_extended_header_available(void const * header_data) {
    assert(header_data != NULL);
    return SFC_HEADER_DEVELOPER_ID_CONST(header_data) == SFC_EXTENDED_HEADER_AVAILABLE;
}

bool sfc_header_extended_available(struct sfc_header const * const header) {
    return header->has_extended;
}

void sfc_header_title(struct sfc_header const * const header, char title[SFC_HEADER_TITLE_MAX_SIZE + 1])
{
    assert(header != NULL);
    assert(header->data != NULL);
    assert(title != NULL);
    void const * const restrict data = header->data;
    size_t const size = find_title_size(SFC_HEADER_TITLE_CONST(data));
    memcpy(title, SFC_HEADER_TITLE_CONST(data), size);
    title[size] = '\0';
}

bool sfc_header_set_title(struct sfc_header const * const header, char title[])
{
    assert(header != NULL);
    assert(header->data != NULL);
    assert(title != NULL);
    void * const restrict data = header->data;
    size_t const size = strlen(title);
    if (size > SFC_HEADER_TITLE_MAX_SIZE)
        return false;
    for (size_t i = 0; i < size; i++)
    {
        if (title[i] < ' ' || title[i] > '~')
            return false;
    }
    memcpy(SFC_HEADER_TITLE(data), title, size);
    memset(SFC_HEADER_TITLE(data) + size, ' ', SFC_HEADER_TITLE_MAX_SIZE - size);
    return true;
}

enum sfc_speed sfc_header_speed(struct sfc_header const * const header)
{
    assert(header != NULL);
    assert(header->data != NULL);
    void const * const restrict data = header->data;
    return (SFC_HEADER_MAP_MODE_CONST(data) & 0x10) > 0 ? SFC_FAST : SFC_SLOW;
}

void sfc_header_set_speed(struct sfc_header const * const header, enum sfc_speed const speed)
{
    assert(header != NULL);
    assert(header->data != NULL);
    void * const restrict data = header->data;
    uint8_t const value = speed == SFC_FAST ? 1 : 0;
    uint8_t mode = SFC_HEADER_MAP_MODE(data);
    mode &= ~0x10;
    mode |= value << 4;
    SFC_HEADER_MAP_MODE(data) = mode;
}

bool sfc_header_map(struct sfc_header const * const header, enum sfc_map * const map)
{
    assert(header != NULL);
    assert(header->data != NULL);
    void const * const restrict data = header->data;
    switch (SFC_HEADER_MAP_MODE_CONST(data) & 0xF)
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

bool sfc_header_set_map(struct sfc_header const * const header, enum sfc_map const map)
{
    assert(header != NULL);
    assert(header->data != NULL);
    void * const restrict data = header->data;
    uint8_t mode = SFC_HEADER_MAP_MODE(data);
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
    SFC_HEADER_MAP_MODE(data) = mode;
    return true;
}

bool sfc_header_cartridge_type(struct sfc_header const * const header, struct sfc_cartridge_type * const cartridge_type)
{
    assert(header != NULL);
    assert(header->data != NULL);
    void const * const restrict data = header->data;
    struct sfc_cartridge_type output = {
        false,
        false,
        false
    };
    switch (SFC_HEADER_CARTRIDGE_TYPE_CONST(data) & 0xF)
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

bool sfc_header_set_cartridge_type(struct sfc_header const * const header, struct sfc_cartridge_type const cartridge_type)
{
    assert(header != NULL);
    assert(header->data != NULL);
    void * const restrict data = header->data;
    int8_t const new_id = cartridge_type_id(cartridge_type);
    if (new_id == -1)
        return false;
    uint8_t id = SFC_HEADER_CARTRIDGE_TYPE(data);
    id &= ~0xF;
    id |= (new_id & 0xF);
    SFC_HEADER_CARTRIDGE_TYPE(data) = id;
    return true;
}

uint32_t sfc_header_rom_size(struct sfc_header const * const header)
{
    assert(header != NULL);
    assert(header->data != NULL);
    void const * const restrict data = header->data;
    return 1 << SFC_HEADER_ROM_SIZE_CONST(data);
}

bool sfc_header_set_rom_size(struct sfc_header const * const header, uint32_t const size)
{
    assert(header != NULL);
    assert(header->data != NULL);
    void * const restrict data = header->data;
    uint32_t const value = find_last_set(size);
    if ((size & ~(1 << value)) != 0)
        return false;
    SFC_HEADER_ROM_SIZE(data) = (uint8_t) value;
    return true;
}

uint32_t sfc_header_ram_size(struct sfc_header const * const header)
{
    assert(header != NULL);
    assert(header->data != NULL);
    void const * const restrict data = header->data;
    return 1 << SFC_HEADER_RAM_SIZE_CONST(data);
}

bool sfc_header_set_ram_size(struct sfc_header const * const header, uint32_t const size)
{
    assert(header != NULL);
    assert(header->data != NULL);
    void * const restrict data = header->data;
    uint32_t const value = find_last_set(size);
    if ((size & ~(1 << value)) != 0)
        return false;
    SFC_HEADER_RAM_SIZE(data) = (uint8_t) value;
    return true;
}

bool sfc_header_destination_code(struct sfc_header const * const header, enum sfc_destination_code * const destination_code)
{
    assert(header != NULL);
    assert(header->data != NULL);
    void const * const restrict data = header->data;
    switch (SFC_HEADER_DESTINATION_CODE_CONST(data))
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

bool sfc_header_set_destination_code(struct sfc_header const * const header, enum sfc_destination_code const destination_code)
{
    assert(header != NULL);
    assert(header->data != NULL);
    void * const restrict data = header->data;
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
    SFC_HEADER_DESTINATION_CODE(data) = value;
    return true;
}

uint8_t sfc_header_developer_id(struct sfc_header const * const header) {
    assert(header != NULL);
    assert(header->data != NULL);
    void const * const restrict data = header->data;
    return SFC_HEADER_DEVELOPER_ID_CONST(data);
}

void sfc_header_set_developer_id(struct sfc_header const * const header, uint8_t const developer_id) {
    assert(header != NULL);
    assert(header->data != NULL);
    void * const restrict data = header->data;
    SFC_HEADER_DEVELOPER_ID(data) = developer_id;
}

uint8_t sfc_header_version(struct sfc_header const * const header) {
    assert(header != NULL);
    assert(header->data != NULL);
    void const * const restrict data = header->data;
    return SFC_HEADER_VERSION_CONST(data);
}

void sfc_header_set_version(struct sfc_header const * const header, uint8_t const version) {
    assert(header != NULL);
    assert(header->data != NULL);
    void * const restrict data = header->data;
    SFC_HEADER_VERSION(data) = version;
}

uint16_t sfc_header_checksum(struct sfc_header const * const header) {
    assert(header != NULL);
    assert(header->data != NULL);
    void const * const restrict data = header->data;
    return SFC_HEADER_CHECKSUM_CONST(data);
}

void sfc_header_set_checksum(struct sfc_header const * const header, uint16_t const checksum) {
    assert(header != NULL);
    assert(header->data != NULL);
    void * const restrict data = header->data;
    SFC_HEADER_CHECKSUM(data) = checksum;
}

bool sfc_header_maker_code(struct sfc_header const * const header, char maker_code[SFC_HEADER_MAKER_CODE_SIZE + 1]) {
    assert(header != NULL);
    assert(header->data != NULL);
    assert(maker_code != NULL);
    if (!header->has_extended)
        return false;
    void const * const restrict data = header->data;
    memcpy(maker_code, SFC_HEADER_MAKER_CODE_CONST(data), 2);
    maker_code[2] = '\0';
    return true;
}

bool sfc_header_set_maker_code(struct sfc_header const * const header, char const maker_code[]) {
    assert(header != NULL);
    assert(header->data != NULL);
    assert(maker_code != NULL);
    if (!header->has_extended || strlen(maker_code) != 2)
        return false;
    for (size_t i = 0; i < 2; i++) {
        if (maker_code[i] < 'A' || maker_code[i] > 'Z')
            return false;
    }
    void * const restrict data = header->data;
    memcpy(SFC_HEADER_MAKER_CODE(data), maker_code, 2);
    return true;
}

bool sfc_header_game_code(struct sfc_header const * const header, char game_code[SFC_HEADER_GAME_CODE_SIZE + 1]) {
    assert(header != NULL);
    assert(header->data != NULL);
    assert(game_code != NULL);
    if (!header->has_extended)
        return false;
    void const * const restrict data = header->data;
    size_t const size = SFC_HEADER_GAME_CODE_CONST(data)[2] != ' ' ? 4 : 2;
    memcpy(game_code, SFC_HEADER_GAME_CODE_CONST(data), size);
    game_code[size] = '\0';
    return true;
}

bool sfc_header_set_game_code(struct sfc_header const * const header, char game_code[]) {
    assert(header != NULL);
    assert(header->data != NULL);
    assert(game_code != NULL);
    if (!header->has_extended)
        return false;
    void * const restrict data = header->data;
    size_t const size = strlen(game_code);
    if (size != SFC_HEADER_GAME_CODE_SHORT_SIZE && size != SFC_HEADER_GAME_CODE_SIZE)
        return false;
    // TODO: Fix check not being removed for valid constants
    for (size_t i = 0; i < size; i++) {
        if (game_code[i] < 'A' || game_code[i] > 'Z')
            return false;
    }
    memcpy(SFC_HEADER_GAME_CODE(data), game_code, size);
    memset(SFC_HEADER_GAME_CODE(data) + size, ' ', SFC_HEADER_GAME_CODE_SIZE - size);
    return true;
}

bool sfc_header_expansion_ram_size(struct sfc_header const * const header, uint32_t *size) {
    assert(header != NULL);
    assert(header->data != NULL);
    if (!header->has_extended)
        return false;
    void const * const restrict data = header->data;
    *size =  1 << SFC_HEADER_EXPANSION_RAM_SIZE_CONST(data);
    return true;
}

bool sfc_header_set_expansion_ram_size(struct sfc_header const * const header, uint32_t const size) {
    assert(header != NULL);
    assert(header->data != NULL);
    if (!header->has_extended)
        return false;
    void * const restrict data = header->data;
    uint32_t const value = find_last_set(size);
    if ((size & ~(1 << value)) != 0)
        return false;
    SFC_HEADER_EXPANSION_RAM_SIZE(data) = (uint8_t) value;
    return true;
}

bool sfc_header_special_version(struct sfc_header const * const header, uint8_t * const version) {
    assert(header != NULL);
    assert(header->data != NULL);
    if (!header->has_extended)
        return false;
    void const * const restrict data = header->data;
    *version = SFC_HEADER_SPECIAL_VERSION_CONST(data);
    return true;
}

bool sfc_header_set_special_version(struct sfc_header const * const header, uint8_t const version) {
    assert(header != NULL);
    assert(header->data != NULL);
    if (!header->has_extended)
        return false;
    void * const restrict data = header->data;
    SFC_HEADER_SPECIAL_VERSION(data) = version;
    return true;
}

bool sfc_header_cartridge_subtype(struct sfc_header const * const header, uint8_t * const type) {
    assert(header != NULL);
    assert(header->data != NULL);
    if (!header->has_extended)
        return false;
    void const * const restrict data = header->data;
    *type = SFC_HEADER_CARTRIDGE_SUBTYPE_CONST(data);
    return true;
}

bool sfc_header_set_cartridge_subtype(struct sfc_header const * const header, uint8_t const type) {
    assert(header != NULL);
    assert(header->data != NULL);
    if (!header->has_extended)
        return false;
    void * const restrict data = header->data;
    SFC_HEADER_CARTRIDGE_SUBTYPE(data) = type;
    return true;
}
