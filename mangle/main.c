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
#include <sfc.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include "../src/header.h"
#include "../src/offset.h"

static char const text[] = "THIS DUMMY ROM IS USED FOR TESTING LIBSFC, AND CANNOT BE EXECUTED!";

static void mangle_rom(struct sfc_rom const * const rom) {
    struct sfc_header const * const header = sfc_rom_header(rom);
    uint16_t const checksum = sfc_header_checksum(header);
    srand(checksum);
    char header_backup[0xFF];
    memcpy(header_backup, header->data, 0xFF);
    size_t const text_chunk_size = rom->memory_size < 1024 ? rom->memory_size : 1024;
    for (size_t i = 0; i < text_chunk_size; i += sizeof(text)) {
        memcpy(OFFSET_POINTER(rom->memory, i), text,
            sizeof(text) > text_chunk_size - i ? text_chunk_size - i : sizeof(text));
    }
    for (size_t i = text_chunk_size; i < rom->memory_size; i++) {
        *((uint8_t*)OFFSET_POINTER(rom->memory, i)) = (uint8_t) rand();
    }

    memcpy(header->data, header_backup, 0xFF);
    sfc_header_set_checksum(header, sfc_checksum(rom));
}

static bool write_rom(struct sfc_rom const * const rom, char const *file) {
    FILE * const output_file = fopen(file, "w");
    if (output_file == NULL) {
        perror("Failed to create the output ROM file");
        return false;
    }
    fwrite(rom->data, rom->size, 1, output_file);
    fclose(output_file);
    return true;
}

int main(int const argc, char const * const argv[]) {
    if (argc < 3) {
        fputs("No arguments specified: mangle INPUT OUTPUT\n", stderr);
        return 1;
    }
    struct sfc_rom const * const rom = sfc_read_rom(argv[1], NULL, NULL);
    if (rom == NULL)
        return 1;
    mangle_rom(rom);
    if (!write_rom(rom, argv[2]))
        return 1;
    return 0;
}
