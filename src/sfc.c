#include <stdio.h>
#include <stdlib.h>

#include "map.h"


struct sfc_rom *sfc_create_rom(const void * const data, const size_t size, const sfc_bool * const copier, const enum sfc_map * const map)
{
    sfc_bool const final_copier = copier != NULL ? *copier : sfc_introspect_copier(size);
    size_t const memory_size = size - (final_copier ? SFC_COPIER_SIZE : 0);
    const void * const memory_data = ((char*)data) + (final_copier ? SFC_COPIER_SIZE : 0);
    enum sfc_map const final_map = map != NULL ? *map : sfc_introspect_map(memory_data, memory_size);
    void * output_data;
    struct sfc_rom * output_rom;

    if (!sfc_header_available(final_map, memory_size))
        return NULL;

    output_data = malloc(size);

    if (output_data == NULL)
        return NULL;

    output_rom = malloc(sizeof(struct sfc_rom));

    if (output_rom == NULL)
        goto error_free_data;

    output_rom->data = output_data;
    output_rom->size = size;

    return output_rom;

error_free_data:
    free(output_data);
    return NULL;
}

void sfc_destroy_rom(struct sfc_rom * rom)
{
    free(rom->data);
    rom->data = NULL;
    free(rom);
}
