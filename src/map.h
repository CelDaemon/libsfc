#ifndef _SFC_MAP_H
#define _SFC_MAP_H

#include <stddef.h>
#include <sfc.h>

#define SFC_COPIER_SIZE 512

size_t sfc_header_offset(enum sfc_map);

bool sfc_introspect_copier(size_t size);

bool sfc_header_available(enum sfc_map map, size_t size);

enum sfc_map sfc_introspect_map(const void *data, size_t size);

#endif

