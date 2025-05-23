#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sfc/rom.h>
#include <sfc/checksum.h>
#include <sfc/header.h>


#if defined(_WIN32)
#  define DIR_SEP '\\'
#else
#  define DIR_SEP '/'
#endif


static int err_sentinel(const int x, const char *msg) {
    if (x != -1) return x;
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}
static void *err_pointer(void * const x, const char *msg) {
    if (x != NULL) return x;
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}
static bool err_boolean(const bool x, const char *msg) {
    if (x != false) return x;
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

int main(const size_t argc, const char *argv[]) {

    if (argc < 3) {
        fprintf(stderr, "No path provided\n");
        const char* name = strrchr(argv[0], DIR_SEP);
        if (name != NULL)
            name++;
        else
            name = argv[0];
        fprintf(stderr, "Usage: %s [INPUT] [OUTPUT]\n", name);
        exit(1);
    }

    struct sfc_rom rom;
    err_boolean(sfc_load_rom(argv[1], SFC_MAP_LO, true, &rom), "Could not load rom");

    printf("Checksum: %.4X\n", sfc_checksum(&rom));

    sfc_header *const header = err_pointer(sfc_rom_header(&rom), "Could not get rom header");

    char title[SFC_MAX_TITLE_LENGTH + 1];
    err_boolean(sfc_header_title(header, title), "Could not get rom title");

    printf("Current title: %s\n", title);

    const char new_title[] = "Test World";
    printf("Setting to: %s\n", new_title);

    err_boolean(sfc_header_set_title(header, new_title), "Could not set rom title");

    err_boolean(sfc_header_title(header, title), "Could not get rom title");

    printf("New title: %s\n", title);

    sfc_header_set_speed(header, SFC_SPD_FAST);
    // err_boolean(sfc_header_set_map(header, SFC_MAP_EX_HI), "Failed to write map");

    printf("Speed: %s\n", sfc_header_speed(header) == SFC_SPD_FAST ? "fast" : "slow");

    err_boolean(sfc_save_rom(&rom, argv[2]), "Could not save rom");

    err_boolean(sfc_header_set_chipset(header, SFC_CHP_RAM | SFC_CHP_BATTERY), "Could not set chipset");
    enum sfc_chipset const chipset = err_sentinel(sfc_header_chipset(header), "Failed to get chipset");

    printf("Has coprocessor: %s, ram: %s, battery: %s\n", (chipset & SFC_CHP_COPROCESSOR) != 0 ? "yes" : "no", (chipset & SFC_CHP_RAM) != 0 ? "yes" : "no", (chipset & SFC_CHP_BATTERY) != 0 ? "yes" : "no");

    printf("Index: %d, Rom size: %d\n", SFC_HDR_ROM_SIZE(header), sfc_header_rom_size(header));

    err_boolean(sfc_header_set_rom_size(header, 1024), "Could not set rom size");
    printf("Index: %d, Rom size: %d\n", SFC_HDR_ROM_SIZE(header), sfc_header_rom_size(header));

    printf("Index: %d, Ram size: %d\n", SFC_HDR_ROM_SIZE(header), sfc_header_ram_size(header));

    err_boolean(sfc_header_set_ram_size(header, 1024), "Could not set ram size");
    printf("Index: %d, Ram size: %d\n", SFC_HDR_ROM_SIZE(header), sfc_header_ram_size(header));

    printf("Country: %d\n", sfc_header_country(header));

    printf("Version: %d\n", sfc_header_version(header));

    sfc_header_set_version(header, 2);
    printf("Version: %d\n", sfc_header_version(header));

    sfc_header_set_checksum(header, sfc_checksum(&rom));

    sfc_save_rom(&rom, argv[2]);

    sfc_unload_rom(&rom);
}
