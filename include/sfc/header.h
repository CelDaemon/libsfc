#pragma once
#include <stdbool.h>
#include <stdint.h>

#include <sfc/rom.h>

#define SFC_HDR_TITLE_OFFSET 0x0 // char[SFC_MAX_TITLE_LENGTH]
#define SFC_HDR_ROM_MODE_OFFSET 0x15 // uint8_t
#define SFC_HDR_CHIPSET_OFFSET 0x16 // uint8_t
#define SFC_HDR_ROM_SIZE_OFFSET 0x17 // uint8_t
#define SFC_HDR_RAM_SIZE_OFFSET 0x18 // uint8_t
#define SFC_HDR_COUNTRY_OFFSET 0x19 // uint8_t
#define SFC_HDR_DEVELOPER_OFFSET 0x1A // uint8_t
#define SFC_HDR_VERSION_OFFSET 0x1B // uint8_t
#define SFC_HDR_CHECKSUM_COMPLEMENT_OFFSET 0x1C // uint16_t
#define SFC_HDR_CHECKSUM_OFFSET 0x1E // uint16_t
#define SFC_HDR_SIZE 32

#define SFC_HDR_TITLE(x) ((char*)(((uint8_t*)x)+SFC_HDR_TITLE_OFFSET))
#define SFC_HDR_ROM_MODE(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_ROM_MODE_OFFSET)))
#define SFC_HDR_CHIPSET(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_CHIPSET_OFFSET)))
#define SFC_HDR_ROM_SIZE(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_ROM_SIZE_OFFSET)))
#define SFC_HDR_RAM_SIZE(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_RAM_SIZE_OFFSET)))
#define SFC_HDR_COUNTRY(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_COUNTRY_OFFSET)))
#define SFC_HDR_DEVELOPER(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_DEVELOPER_OFFSET)))
#define SFC_HDR_VERSION(x) (*((uint8_t*)(((uint8_t*)x)+SFC_HDR_VERSION_OFFSET)))
#define SFC_HDR_CHECKSUM_COMPLEMENT(x) (*((uint16_t*)(((uint8_t*)x)+SFC_HDR_CHECKSUM_COMPLEMENT_OFFSET)))
#define SFC_HDR_CHECKSUM(x) (*((uint16_t*)(((uint8_t*)x)+SFC_HDR_CHECKSUM_OFFSET)))

#define SFC_MAX_TITLE_LENGTH 21

enum sfc_speed {
    SFC_SPD_SLOW,
    SFC_SPD_FAST,
    SFC_SPD__FIRST = SFC_SPD_SLOW,
    SFC_SPD__LAST = SFC_SPD_FAST
};

#define SFC_SPD_VALID(speed) ((speed) >= SFC_SPD__FIRST && (speed) <= SFC_SPD__LAST)

enum sfc_chipset {
    SFC_CHP_INVALID = -1,
    SFC_CHP_RAM = 1 << 1,
    SFC_CHP_BATTERY = 1 << 2,
    SFC_CHP_COPROCESSOR = 1 << 3,
    SFC_CHP__MASK = SFC_CHP_RAM | SFC_CHP_BATTERY | SFC_CHP_COPROCESSOR
};

enum sfc_country {
    SFC_CTY_JAPAN,
    SFC_CTY_USA,
    SFC_CTY_EUROPE,
    SFC_CTY_INVALID = -1,
    SFC_CTY__FIRST = SFC_CTY_JAPAN,
    SFC_CTY__LAST = SFC_CTY_EUROPE
};

#define SFC_CTY_VALID(country) ((country) >= SFC_CTY__FIRST && (country) <= SFC_CTY__LAST)

typedef void* sfc_header;




sfc_header *sfc_rom_header(const struct sfc_rom *rom);

bool sfc_header_title(const sfc_header *header, char title[ SFC_MAX_TITLE_LENGTH + 1]);
bool sfc_header_set_title(sfc_header *header, const char *title);

enum sfc_speed sfc_header_speed(const sfc_header *header);
bool sfc_header_set_speed(sfc_header *header, enum sfc_speed speed);

enum sfc_mapping sfc_header_map(const sfc_header *header);
bool sfc_header_set_map(sfc_header *header, enum sfc_mapping map);

enum sfc_chipset sfc_header_chipset(const sfc_header *header);
bool sfc_header_set_chipset(sfc_header *header, enum sfc_chipset chipset);

uint16_t sfc_header_rom_size(const sfc_header *header);
bool sfc_header_set_rom_size(sfc_header *header, uint16_t size);

uint16_t sfc_header_ram_size(const sfc_header *header);
bool sfc_header_set_ram_size(sfc_header *header, uint16_t size);

enum sfc_country sfc_header_country(const sfc_header *header);
bool sfc_header_set_country(sfc_header *header, enum sfc_country country);

uint8_t sfc_header_version(const sfc_header *header);
void sfc_header_set_version(sfc_header *header, uint8_t version);

uint16_t sfc_header_checksum(const sfc_header *header);
void sfc_header_set_checksum(sfc_header *header, uint16_t checksum);
