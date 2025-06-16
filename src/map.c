#include "map.h"

#include <stdlib.h>

#include "header.h"
#include "sfc.h"

/**
 * Get the header offset in ROM memory
 * @param map ROM memory map
 * @return The ROM header offset in memory
 */
size_t sfc_header_offset(enum sfc_map const map)
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

bool sfc_introspect_copier(size_t const size)
{
    return (size & 512) > 0;
}

bool sfc_header_available(enum sfc_map const map, size_t const size)
{
    const size_t offset = sfc_header_offset(map);
    return offset + SFC_HEADER_SIZE < size;
}

enum sfc_map sfc_introspect_map(void const * const data, size_t const size)
{
    (void)data;
    (void)size;
    return SFC_MAP_LO;
}
