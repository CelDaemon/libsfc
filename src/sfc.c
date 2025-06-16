#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "map.h"


struct sfc_rom *sfc_create_rom(const void * const data, size_t const size, const bool * const copier, const enum sfc_map * const map)
{
    assert(data != NULL);
    bool const final_copier = copier != NULL ? *copier : sfc_introspect_copier(size);
    size_t const memory_size = size - (final_copier ? SFC_COPIER_SIZE : 0);
    void const * const memory_data = ((char*)data) + (final_copier ? SFC_COPIER_SIZE : 0);
    enum sfc_map const final_map = map != NULL ? *map : sfc_introspect_map(memory_data, memory_size);

    if (!sfc_header_available(final_map, memory_size))
        return NULL;

    void * const output_data = malloc(size);

    if (output_data == NULL)
        return NULL;

    struct sfc_rom * const output_rom = malloc(sizeof(struct sfc_rom));

    if (output_rom == NULL)
        goto error_free_data;

    output_rom->data = output_data;
    output_rom->size = size;

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
