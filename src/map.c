#include "map.h"

#include <assert.h>
#include <stdlib.h>

#include "header.h"
#include "sfc.h"

/**
 * Get the header offset in ROM memory
 * @param map ROM memory map
 * @return The ROM header offset in memory
 */
size_t sfc_header_offset(const enum sfc_map map)
{
    switch (map)
    {
    case SFC_MAP_LO:
        return 0x007F00;
    case SFC_MAP_HI:
        return 0x00FF00;
    case SFC_MAP_EX_HI:
        return 0x40FF00;
    default:
        abort();
    }
}

bool sfc_introspect_copier(const size_t size)
{
    return (size & 512) > 0;
}

bool sfc_header_available(const enum sfc_map map, const size_t size)
{
    const size_t offset = sfc_header_offset(map);
    return offset + SFC_HEADER_SIZE < size;
}

enum sfc_map sfc_introspect_map(const void *data, const size_t size)
{
    (void)data;
    (void)size;
    return SFC_MAP_LO;
}
