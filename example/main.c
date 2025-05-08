#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sfc.h"

int main(const size_t argc, const char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "No path provided\n");
        exit(1);
    }
    struct sfc_rom rom;
    if (!sfc_load_rom(argv[1], SFC_MAP_LO, true, &rom)) {
        fprintf(stderr, "Could not load rom: %s\n", strerror(errno));
        exit(1);
    }
    char title[max_title_length + 1];
    if (!sfc_rom_title(&rom, title)) {
        fprintf(stderr, "Could not get rom title: %s\n", strerror(errno));
        exit(1);
    }
    printf("Current title: %s\n", title);
    auto const new_title = "Mew Mew Superworld";
    printf("Setting to: %s\n", new_title);
    if (!sfc_set_rom_title(&rom, new_title)) {
        fprintf(stderr, "Could not set rom title: %s\n", strerror(errno));
        exit(1);
    }

    if (!sfc_rom_title(&rom, title)) {
        fprintf(stderr, "Could not get rom title: %s\n", strerror(errno));
        exit(1);
    }
    if (!sfc_save_rom(&rom, argv[2])) {
        fprintf(stderr, "Could not save rom: %s\n", strerror(errno));
        exit(1);
    }
    printf("New title: %s\n", title);

    sfc_unload_rom(&rom);
}

