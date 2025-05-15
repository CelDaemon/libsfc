#pragma once

#include "sfc.h"

#include <stddef.h>
auto constexpr copier_size = 512;

size_t header_offset(enum sfc_map mapping);

size_t data_offset(bool copier);