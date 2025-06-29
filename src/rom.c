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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "header.h"
#include "map.h"
#include "offset.h"

#ifdef _MSC_VER
#define fileno _fileno
#endif

struct sfc_rom *sfc_read_rom(char const * const path, bool const * const copier, enum sfc_map const * const map) {
    assert(path != NULL);
    FILE * const file = fopen(path, "r");
    if (file == NULL)
        return NULL;
    struct stat stat;
    if (fstat(fileno(file), &stat) != 0) {
        fclose(file);
        return NULL;
    }

    size_t capacity = stat.st_size;

    void *data = malloc(capacity);
    size_t written;
    size_t cursor = 0;
#define CHUNK_SIZE 1024
    char buffer[CHUNK_SIZE];
    do {
        written = fread(buffer, 1, CHUNK_SIZE, file);
        if (written > capacity - cursor) {
            size_t const new_capacity = capacity * 2 > cursor + written ? capacity * 2 : cursor + written;
            void * const new_data = realloc(data, new_capacity);
            if (new_data == NULL) {
                fclose(file);
                free(data);
                return NULL;
            }
            capacity = new_capacity;
            data = new_data;
        }
        memcpy(OFFSET_POINTER(data, cursor), buffer, written);
        cursor += written;
    } while (written == CHUNK_SIZE);
#undef CHUNK_SIZE
    if (ferror(file) != 0) {
        fclose(file);
        free(data);
        return NULL;
    }
    fclose(file);

    struct sfc_rom * const rom = sfc_load_rom(data, cursor, copier, map);
    if (rom == NULL)
        free(data);
    return rom;
}

struct sfc_rom *sfc_load_rom(void * const data, size_t const size, const bool * const copier, const enum sfc_map * const map)
{
    assert(data != NULL);
    bool const final_copier = copier != NULL ? *copier : sfc_introspect_copier(size);
    size_t const copier_offset = final_copier ? SFC_COPIER_SIZE : 0;
    size_t const memory_size = size - copier_offset;
    void * const memory = OFFSET_POINTER(data, copier_offset);
    enum sfc_map const final_map = map != NULL ? *map : sfc_introspect_map(memory, memory_size);

    if (!sfc_header_available(final_map, memory_size))
        return NULL;

    struct sfc_rom * const output_rom = malloc(sizeof(struct sfc_rom));

    if (output_rom == NULL)
        return NULL;

    void * const header_data = OFFSET_POINTER(data, copier_offset + sfc_header_offset(final_map));

    struct sfc_header const header = {
        header_data,
        sfc_extended_header_available(header_data)
    };

    struct sfc_rom const rom = {
        data,
        size,
        memory,
        memory_size,
        header
    };
    *output_rom = rom;

    return output_rom;
}

void sfc_destroy_rom(struct sfc_rom * const rom)
{
    if (rom == NULL)
        return;
    free(rom->data);
    free(rom);
}

struct sfc_header const *sfc_rom_header(struct sfc_rom const * const rom)
{
    assert(rom != NULL);
    return &rom->header;
}

void const *sfc_rom_data(struct sfc_rom const * const rom, size_t * const size) {
    assert(rom != NULL);
    assert(size != NULL);
    *size = rom->size;
    return rom->data;
}
