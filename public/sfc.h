#pragma once
#include <stdint.h>
#include <stddef.h>

auto constexpr max_title_length = 21;

enum sfc_map {
    SFC_MAP_SMART, // Unspecified, requires inferring the mapping
    SFC_MAP_LO,
    SFC_MAP_HI,
    SFC_MAP_EX_HI,
    SFC_MAP_INVALID = -1,
    SFC_MAP__CONCRETE_FIRST = SFC_MAP_LO,
    SFC_MAP__CONCRETE_LAST = SFC_MAP_EX_HI,
    SFC_MAP__FIRST = SFC_MAP_SMART,
    SFC_MAP__LAST = SFC_MAP_EX_HI
};

#define SFC_MAP_CONCRETE(mapping) ((mapping) >= SFC_MAP__CONCRETE_FIRST && (mapping) <= SFC_MAP__CONCRETE_LAST)

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

struct sfc_rom {
    void *data;
    size_t size;
    enum sfc_map map;
    bool copier;
};

struct sfc_header {
    char title[max_title_length];
    uint8_t rom_mode;
    uint8_t chipset;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t country;
    uint8_t developer;
    uint8_t version;
    uint16_t checksum_complement;
    uint16_t checksum;
};




bool sfc_load_rom(const char *path, enum sfc_map map, bool copier, struct sfc_rom *rom);
void sfc_unload_rom(struct sfc_rom *rom);
bool sfc_save_rom(const struct sfc_rom *rom, const char *path);

struct sfc_header *sfc_rom_header(const struct sfc_rom *rom);

bool sfc_header_title(const struct sfc_header *header, char title[static max_title_length + 1]);
bool sfc_header_set_title(struct sfc_header *header, const char *title);

enum sfc_speed sfc_header_speed(const struct sfc_header *header);
bool sfc_header_set_speed(struct sfc_header *header, enum sfc_speed speed);

enum sfc_map sfc_header_map(const struct sfc_header *header);
bool sfc_header_set_map(struct sfc_header *header, enum sfc_map map);

enum sfc_chipset sfc_header_chipset(const struct sfc_header *header);
bool sfc_header_set_chipset(struct sfc_header *header, enum sfc_chipset chipset);

uint16_t sfc_header_rom_size(const struct sfc_header *header);
bool sfc_header_set_rom_size(struct sfc_header *header, uint16_t size);