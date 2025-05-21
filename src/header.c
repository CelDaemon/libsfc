#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <assert.h>
#include <limits.h>

#include "last_bit.h"
#include "sfc.h"
#include "mapping.h"

#define bit_set(number, n, x) ((number & ~(1 << (n))) | ((x) << (n)))

static size_t title_length(const char title[ SFC_MAX_TITLE_LENGTH]) {
    assert(title != NULL);
    size_t length = 0;
    for (int i = 0; i < SFC_MAX_TITLE_LENGTH; i++) {
        if (title[SFC_MAX_TITLE_LENGTH - i - 1] == ' ')
            continue;

        length = SFC_MAX_TITLE_LENGTH - i;
        break;
    }
    return length;
}

static bool valid_title(const char *title, const size_t length) {
    assert(title != NULL);
    for (size_t i = 0; i < length; i++) {
        if (!isprint(title[i]))
            return false;
    }
    return true;
}

struct sfc_header *sfc_rom_header(const struct sfc_rom *rom) {
    assert(rom != NULL);
    size_t const offset = sfc_data_offset(rom->copier) + sfc_header_offset(rom->map);
    assert(rom->size > offset + sizeof(struct sfc_header));
    return (struct sfc_header *) (((uint8_t *) rom->data) + offset);
}

bool sfc_header_title(const struct sfc_header *header, char title[ SFC_MAX_TITLE_LENGTH + 1]) {
    assert(header != NULL);
    assert(title != NULL);
    if (!valid_title(header->title, SFC_MAX_TITLE_LENGTH)) {
        errno = EINVAL;
        return false;
    }
    memcpy(title, header->title, SFC_MAX_TITLE_LENGTH);
    title[title_length(title)] = '\0';
    return true;
}

bool sfc_header_set_title(struct sfc_header *header, const char *title) {
    assert(header != NULL);
    assert(title != NULL);
    size_t const length = strlen(title);
    if (length > SFC_MAX_TITLE_LENGTH || !valid_title(title, length)) {
        errno = EINVAL;
        return false;
    }
    memcpy(header->title, title, length);
    memset(header->title + length, ' ', SFC_MAX_TITLE_LENGTH - length);
    return true;
}

enum sfc_speed sfc_header_speed(const struct sfc_header *header) {
    assert(header != NULL);
    return (header->rom_mode & 0x10) > 0 ? SFC_SPD_FAST : SFC_SPD_SLOW;
}

bool sfc_header_set_speed(struct sfc_header *header, const enum sfc_speed speed) {
    assert(header != NULL);
    if (!SFC_SPD_VALID(speed)) {
        errno = EINVAL;
        return false;
    }
    header->rom_mode = bit_set(header->rom_mode, 4, speed == SFC_SPD_FAST ? 1 : 0);
    return true;
}

enum sfc_map sfc_header_map(const struct sfc_header *header) {
    assert(header != NULL);
    switch (header->rom_mode & 0xF) {
        case 0:
            return SFC_MAP_LO;
        case 1:
            return SFC_MAP_HI;
        case 5:
            return SFC_MAP_EX_HI;
        default:
            errno = EINVAL;
            return SFC_MAP_INVALID;
    }
}

bool sfc_header_set_map(struct sfc_header *header, const enum sfc_map map) {
    assert(header != NULL);
    uint8_t value;
    switch (map) {
        case SFC_MAP_LO:
            value = 0;
            break;
        case SFC_MAP_HI:
            value = 1;
            break;
        case SFC_MAP_EX_HI:
            value = 5;
            break;
        default:
            errno = EINVAL;
            return false;
    }
    header->rom_mode = (header->rom_mode & ~0xF) | value;
    return true;
}

enum sfc_chipset sfc_header_chipset(const struct sfc_header *header) {
    assert(header != NULL);
    switch (header->chipset & 0xF) {
        case 0x00:
            return 0;
        case 0x01:
            return SFC_CHP_RAM;
        case 0x02:
            return SFC_CHP_RAM | SFC_CHP_BATTERY;
        case 0x03:
            return SFC_CHP_COPROCESSOR;
        case 0x04:
            return SFC_CHP_COPROCESSOR | SFC_CHP_RAM;
        case 0x05:
            return SFC_CHP_COPROCESSOR | SFC_CHP_RAM | SFC_CHP_BATTERY;
        case 0x06:
            return SFC_CHP_COPROCESSOR | SFC_CHP_BATTERY;
        default:
            errno = EINVAL;
            return SFC_CHP_INVALID;
    }
}

static int chipset_id(const bool ram, const bool battery, const bool coprocessor) {
    if (!coprocessor && !ram && battery)
        return -1;


    if (!coprocessor) {
        if (!ram)
            return 0;
        if (!battery)
            return 1;
        return 2;
    }
    if (!ram) {
        if (!battery)
            return 3;
        return 6;
    }
    if (!battery)
        return 4;
    return 5;
}

bool sfc_header_set_chipset(struct sfc_header *header, const enum sfc_chipset chipset) {
    assert(header != NULL);
    int const value = chipset_id(chipset & SFC_CHP_RAM, chipset & SFC_CHP_BATTERY, chipset & SFC_CHP_COPROCESSOR);
    if (value == -1) {
        errno = EINVAL;
        return false;
    }

    header->chipset = (uint8_t) ((header->chipset & 0xF) | value);
    return true;

}

uint16_t sfc_header_rom_size(const struct sfc_header *header) {
    assert(header != NULL);
    assert(header->rom_size < sizeof(uint16_t) * CHAR_BIT);

    return 1 << header->rom_size;
}

bool sfc_header_set_rom_size(struct sfc_header *header, const uint16_t size) {
    if (size == 0) {
        errno = EINVAL;
        return false;
    }
    size_t const index = last_bit(size);
    if (1 << index != size) {
        errno = EINVAL;
        return false;
    }

    header->rom_size = (uint8_t) index;
    return true;
}

uint16_t sfc_header_ram_size(const struct sfc_header *header) {
    assert(header != NULL);
    assert(header->ram_size < sizeof(uint16_t) * CHAR_BIT);

    return 1 << header->ram_size;
}

bool sfc_header_set_ram_size(struct sfc_header *header, const uint16_t size) {
    if (size == 0) {
        errno = EINVAL;
        return false;
    }
    size_t const index = last_bit(size);
    if (1 << index != size) {
        errno = EINVAL;
        return false;
    }

    header->ram_size = (uint8_t) index;
    return true;
}

enum sfc_country sfc_header_country(const struct sfc_header *header) {
    if (!SFC_CTY_VALID(header->country))
        return SFC_CTY_INVALID;
    return header->country;
}

bool sfc_header_set_country(struct sfc_header *header, const enum sfc_country country) {
    if (!SFC_CTY_VALID(country)) {
        errno = EINVAL;
        return false;
    }
    header->country = country;
    return true;
}

uint8_t sfc_header_version(const struct sfc_header *header) {
    return header->version;
}

void sfc_header_set_version(struct sfc_header *header, const uint8_t version) {
    header->version = version;
}

uint16_t sfc_header_checksum(const struct sfc_header *header) {
    return header->checksum;
}

void sfc_header_set_checksum(struct sfc_header *header, const uint16_t checksum) {
    header->checksum = checksum;
    header->checksum_complement = ~checksum;
}