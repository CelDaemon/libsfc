/*
    SPDX-FileCopyrightText: 2025 CelDaemon <celdaemon@voidgroup.net>

    SPDX-License-Identifier: BSD-3-Clause
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
    if (argc < 2)
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
    struct sfc_rom * const rom = sfc_create_rom(data, stat.st_size, NULL, NULL);
    free(data);
    if (rom == NULL)
    {
        fprintf(stderr, "Failed to load ROM\n");
        return 2;
    }
    printf("Data: %p, Size: %zu\n", rom->data, rom->size);
    sfc_header const header = sfc_rom_header(rom);
    printf("Header: %p\n", rom->header);
    if (!sfc_header_set_title(header, "MEOW A"))
    {
        fprintf(stderr, "Failed to set title\n");
        sfc_destroy_rom(rom);
        return 1;
    };
    char title[22];
    sfc_header_title(header, title);
    printf("Title: %s\n", title);
    sfc_header_set_speed(header, SFC_FAST);
    printf("ROM Speed: %d\n", sfc_header_speed(header));
    if (!sfc_header_set_map(header, SFC_MAP_EX_HI))
    {
        fprintf(stderr, "Failed to set map\n");
        sfc_destroy_rom(rom);
        return 1;
    };
    enum sfc_map map;
    if (!sfc_header_map(header, &map))
    {
        fprintf(stderr, "Failed to get map\n");
        sfc_destroy_rom(rom);
        return 1;
    }
    printf("ROM Map: %d\n", map);
    struct sfc_chipset const new_chipset = {
        true,
        true,
        false
    };
    if (!sfc_header_set_chipset(header, new_chipset))
    {
        fprintf(stderr, "Failed to set chipset\n");
        sfc_destroy_rom(rom);
        return 1;
    }
    struct sfc_chipset chipset;
    if (!sfc_header_chipset(header, &chipset))
    {
        fprintf(stderr, "Failed to get chipset\n");
        sfc_destroy_rom(rom);
        return 1;
    }
    printf("SRAM: %d, battery: %d, coprocessor: %d\n", chipset.ram, chipset.battery, chipset.coprocessor);
    if (!sfc_header_set_rom_size(header, 2048))
    {
        fprintf(stderr, "Failed to set ROM size\n");
        sfc_destroy_rom(rom);
        return 1;
    }
    if (!sfc_header_set_ram_size(header, 2048))
    {
        fprintf(stderr, "Failed to set RAM size\n");
        sfc_destroy_rom(rom);
        return 1;
    }

    printf("ROM size: %zu KiB, SRAM size: %zu KiB", sfc_header_rom_size(header), sfc_header_ram_size(header));
    sfc_destroy_rom(rom);
    return 0;
}
