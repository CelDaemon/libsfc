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

bool sfc_deduce_copier(size_t const size)
{
    return (size & 512) > 0;
}

bool sfc_header_available(enum sfc_map const map, size_t const size)
{
    const size_t offset = sfc_header_offset(map);
    return offset + 0xFF < size;
}

enum sfc_map sfc_deduce_map(void const * const data, size_t const size)
{
    assert(data != NULL);
    (void)data;
    (void)size;
    return SFC_MAP_LO;
}
