#include <stdio.h>

#include "map.h"


struct sfc_rom *sfc_create_rom(const void * const data, const size_t size, const sfc_bool * const copier, const enum sfc_map * const map)
{
    sfc_bool const final_copier = copier != NULL ? *copier : sfc_introspect_copier(size);
    size_t const memory_size = size - (final_copier ? SFC_COPIER_SIZE : 0);
    const void * const memory_data = ((char*)data) + (final_copier ? SFC_COPIER_SIZE : 0);
    enum sfc_map const final_map = map != NULL ? *map : sfc_introspect_map(memory_data, memory_size);

    if (!sfc_header_available(final_map, memory_size))
        return NULL;




}
