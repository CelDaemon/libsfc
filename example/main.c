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
    sfc_header_set_title(header, "meow");
    char title[22];
    sfc_header_title(header, title);
    printf("Title: %s\n", title);
    sfc_header_set_speed(header, SFC_FAST);
    printf("ROM Speed: %d\n", sfc_header_speed(header));
    sfc_header_set_map(header, SFC_MAP_EX_HI);
    printf("ROM Map: %d\n", sfc_header_map(header));
    struct sfc_chipset const new_chipset = {
        false,
        true,
        false
    };
    sfc_header_set_chipset(header, new_chipset);
    struct sfc_chipset const chipset = sfc_header_chipset(header);
    printf("SRAM: %d, battery: %d, coprocessor: %d\n", chipset.ram, chipset.battery, chipset.coprocessor);
    sfc_destroy_rom(rom);
    return 0;
}
