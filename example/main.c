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

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <sfc.h>

#ifdef _MSC_VER
#define fileno _fileno
#endif

int main(int const argc, char const * const argv[])
{
    if (argc < 3)
        return 1;
    FILE * const file = fopen(argv[1], "r");
    if (file == NULL)
        return 2;
    struct stat stat;
    fstat(fileno(file), &stat);
    void * data = malloc(stat.st_size);
    if (fread(data, stat.st_size, 1, file) != 1)
    {
        fprintf(stderr, "Failed to read ROM file\n");
        fclose(file);
        free(data);
        return 2;
    }
    fclose(file);
    enum sfc_map const load_map = SFC_MAP_LO;
    struct sfc_rom * const rom = sfc_create_rom(data, stat.st_size, NULL, &load_map);
    free(data);
    if (rom == NULL)
    {
        fprintf(stderr, "Failed to load ROM\n");
        return 2;
    }
    printf("Data: %p, Size: %zu\n", rom->data, rom->size);
    printf("Calculated checksum: %hX\n", sfc_checksum(rom));
    struct sfc_header const * const header = sfc_rom_header(rom);
    printf("Header: %p\n", header->data);
    if (!sfc_header_set_title(header, "MEOW A"))
    {
        fprintf(stderr, "Failed to set title\n");
        goto error;
    };
    char title[SFC_HEADER_TITLE_MAX_SIZE + 1];
    sfc_header_title(header, title);
    printf("Title: %s\n", title);
    sfc_header_set_speed(header, SFC_FAST);
    printf("ROM Speed: %d\n", sfc_header_speed(header));
    if (!sfc_header_set_map(header, SFC_MAP_EX_HI))
    {
        fprintf(stderr, "Failed to set map\n");
        goto error;
    };
    enum sfc_map map;
    if (!sfc_header_map(header, &map))
    {
        fprintf(stderr, "Failed to get map\n");
        goto error;
    }
    printf("ROM Map: %d\n", map);
    struct sfc_cartridge_type const new_cartridge_type = {
        true,
        true,
        false
    };
    if (!sfc_header_set_cartridge_type(header, new_cartridge_type))
    {
        fprintf(stderr, "Failed to set chipset\n");
        goto error;
    }
    struct sfc_cartridge_type cartridge_type;
    if (!sfc_header_cartridge_type(header, &cartridge_type))
    {
        fprintf(stderr, "Failed to get chipset\n");
        goto error;
    }
    printf("SRAM: %d, battery: %d, coprocessor: %d\n", cartridge_type.ram, cartridge_type.battery, cartridge_type.coprocessor);
    if (!sfc_header_set_rom_size(header, 2048))
    {
        fprintf(stderr, "Failed to set ROM size\n");
        goto error;
    }
    if (!sfc_header_set_ram_size(header, 2048))
    {
        fprintf(stderr, "Failed to set RAM size\n");
        goto error;
    }

    printf("ROM size: %u KiB, SRAM size: %u KiB\n", sfc_header_rom_size(header), sfc_header_ram_size(header));


    if (!sfc_header_set_destination_code(header, SFC_DESTINATION_AUSTRALIA))
    {
        fprintf(stderr, "Failed to set destination code\n");
        goto error;
    }

    FILE * const output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        perror("meow");
        fprintf(stderr, "Failed to open output file\n");
        goto error;
    }
    fwrite(rom->data, rom->size, 1, output_file);
    fclose(output_file);
    sfc_destroy_rom(rom);
    return 0;
error:
    sfc_destroy_rom(rom);
    return 1;
}
