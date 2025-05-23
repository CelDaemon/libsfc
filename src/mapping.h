#pragma once

#include <stdbool.h>
#include <stddef.h>

#include <sfc/rom.h>

size_t sfc_header_offset(enum sfc_mapping mapping);

size_t sfc_data_offset(bool copier);
