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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "offset.h"


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
