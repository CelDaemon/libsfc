#include <stdio.h>
#include <string.h>

#include "sfc.h"

int main(int const argc, const char * const argv[])
{
    struct sfc_rom * rom;
    if (argc < 2)
        return 1;
    rom = sfc_create_rom(argv[1], strlen(argv[1]), NULL, NULL);
    if (rom == NULL)
        return 1;
    printf("Data: %p, Size: %lu", rom->data, (unsigned long) rom->size);
    sfc_destroy_rom(rom);
    return 0;
}
