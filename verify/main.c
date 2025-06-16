#include <stdio.h>
#include <string.h>

#include "sfc.h"

int main(int const argc, char const * const argv[])
{
    if (argc < 2)
        return 1;
    struct sfc_rom * const rom = sfc_create_rom(argv[1], strlen(argv[1]), NULL, NULL);
    if (rom == NULL)
        return 1;
    printf("Data: %p, Size: %zu", rom->data, rom->size);
    sfc_destroy_rom(rom);
    return 0;
}
