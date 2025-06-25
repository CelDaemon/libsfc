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
    // May enable 50fps PAL mode
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

struct sfc_header {
    void *data;
    bool has_extended;
};

struct sfc_rom
{
    void *data;
    size_t size;
    void *memory;
    size_t memory_size;
    struct sfc_header header;
};


struct sfc_rom *sfc_create_rom(void const *input_data, size_t size, bool const *copier, enum sfc_map const *map);
void sfc_destroy_rom(struct sfc_rom *rom);
struct sfc_header const *sfc_rom_header(struct sfc_rom const *rom);

char *sfc_header_title(struct sfc_header const *header, char title[SFC_HEADER_TITLE_MAX_SIZE + 1]);
bool sfc_header_set_title(struct sfc_header const *header, char title[]);

enum sfc_speed sfc_header_speed(struct sfc_header const *header);
void sfc_header_set_speed(struct sfc_header const *header, enum sfc_speed speed);

bool sfc_header_map(struct sfc_header const *header, enum sfc_map *map);
bool sfc_header_set_map(struct sfc_header const *header, enum sfc_map map);

bool sfc_header_cartridge_type(struct sfc_header const *header, struct sfc_cartridge_type *cartridge_type);
bool sfc_header_set_cartridge_type(struct sfc_header const *header, struct sfc_cartridge_type cartridge_type);

uint32_t sfc_header_rom_size(struct sfc_header const *header);
bool sfc_header_set_rom_size(struct sfc_header const *header, uint32_t size);

uint32_t sfc_header_ram_size(struct sfc_header const *header);
bool sfc_header_set_ram_size(struct sfc_header const *header, uint32_t size);

bool sfc_header_destination_code(struct sfc_header const *header, enum sfc_destination_code *destination_code);
bool sfc_header_set_destination_code(struct sfc_header const *header, enum sfc_destination_code destination_code);

uint8_t sfc_header_developer_id(struct sfc_header const *header);
void sfc_header_set_developer_id(struct sfc_header const *header, uint8_t developer_id);

uint8_t sfc_header_version(struct sfc_header const *header);
void sfc_header_set_version(struct sfc_header const *header, uint8_t version);

uint16_t sfc_header_checksum(struct sfc_header const *header);
void sfc_header_set_checksum(struct sfc_header const *header, uint16_t checksum);

uint16_t sfc_checksum(struct sfc_rom const *rom);

#endif
