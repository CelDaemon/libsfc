/*
    SPDX-FileCopyrightText: 2025 CelDaemon <celdaemon@voidgroup.net>

    SPDX-License-Identifier: BSD-3-Clause
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "util.h"


struct sfc_rom *sfc_create_rom(const void * const input_data, size_t const size, const bool * const copier, const enum sfc_map * const map)
{
    assert(input_data != NULL);
    bool const final_copier = copier != NULL ? *copier : sfc_introspect_copier(size);
    size_t const copier_offset = final_copier ? SFC_COPIER_SIZE : 0;
    size_t const memory_size = size - copier_offset;
    void const * const input_memory_data = OFFSET_POINTER(input_data, copier_offset);
    enum sfc_map const final_map = map != NULL ? *map : sfc_introspect_map(input_memory_data, memory_size);

    if (!sfc_header_available(final_map, memory_size))
        return NULL;

    void * const output_data = malloc(size);

    if (output_data == NULL)
        return NULL;

    struct sfc_rom * const output_rom = malloc(sizeof(struct sfc_rom));

    if (output_rom == NULL)
        goto error_free_data;

    memcpy(output_data, input_data, size);

    output_rom->data = output_data;
    output_rom->size = size;
    output_rom->header = OFFSET_POINTER(output_data, copier_offset + sfc_header_offset(final_map));

    return output_rom;

error_free_data:
    free(output_data);
    return NULL;
}

void sfc_destroy_rom(struct sfc_rom * const rom)
{
    if (rom == NULL)
        return;
    free(rom->data);
    rom->data = NULL;
    free(rom);
}

sfc_header sfc_rom_header(struct sfc_rom const * const rom)
{
    assert(rom != NULL);
    return rom->header;
}
