#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sfc.h"


#define err(run, msg, check) ({ auto const result = (run); if((check)) { fprintf(stderr, msg ": %s\n", strerror(errno)); exit(1); } result; })
#define err_b(run, msg) err(run, msg, !result) // Boolean
#define err_p(run, msg) err(run, msg, result == nullptr) // Pointer
#define err_s(run, msg) err(run, msg, result == -1) // Sentinel

int main(const size_t argc, const char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "No path provided\n");
        exit(1);
    }

    struct sfc_rom rom;
    err_b(sfc_load_rom(argv[1], SFC_MAP_LO, true, &rom), "Could not load rom");

    auto const header = err_p(sfc_rom_header(&rom), "Could not get rom header");

    char title[max_title_length + 1];
    err_b(sfc_header_title(header, title), "Could not get rom title");

    printf("Current title: %s\n", title);

    auto const new_title = "Test World";
    printf("Setting to: %s\n", new_title);

    err_b(sfc_header_set_title(header, new_title), "Could not set rom title");

    err_b(sfc_header_title(header, title), "Could not get rom title");

    printf("New title: %s\n", title);

    sfc_header_set_speed(header, SFC_SPD_FAST);
    err_b(sfc_header_set_map(header, SFC_MAP_EX_HI), "Failed to write map");

    printf("Speed: %s\n", sfc_header_speed(header) == SFC_SPD_FAST ? "fast" : "slow");

    err_b(sfc_save_rom(&rom, argv[2]), "Could not save rom");

    err_b(sfc_header_set_chipset(header, SFC_CHP_RAM | SFC_CHP_BATTERY), "Could not set chipset");
    auto const chipset = err_s(sfc_header_chipset(header), "Failed to get chipset");

    printf("Has coprocessor: %s, ram: %s, battery: %s\n", (chipset & SFC_CHP_COPROCESSOR) != 0 ? "yes" : "no", (chipset & SFC_CHP_RAM) != 0 ? "yes" : "no", (chipset & SFC_CHP_BATTERY) != 0 ? "yes" : "no");

    printf("Index: %d, Rom size: %d\n", header->rom_size, sfc_header_rom_size(header));

    err_b(sfc_header_set_rom_size(header, 1024), "Could not set rom size");
    printf("Index: %d, Rom size: %d\n", header->rom_size, sfc_header_rom_size(header));

    printf("Index: %d, Ram size: %d\n", header->rom_size, sfc_header_ram_size(header));

    err_b(sfc_header_set_ram_size(header, 1024), "Could not set ram size");
    printf("Index: %d, Ram size: %d\n", header->rom_size, sfc_header_ram_size(header));

    printf("Country: %d\n", sfc_header_country(header));

    printf("Version: %d\n", sfc_header_version(header));

    sfc_header_set_version(header, 2);
    printf("Version: %d\n", sfc_header_version(header));

    sfc_unload_rom(&rom);
}

