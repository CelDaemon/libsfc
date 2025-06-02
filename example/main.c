

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sfc/rom.h>
#include <sfc/checksum.h>
#include <sfc/header.h>



#ifdef _WIN32
#  define DIR_SEP '\\'
#else
#  define DIR_SEP '/'
#endif

int main(const int argc, const char *argv[]) {
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
    size_t rom_size;
    void *rom_data = sfc_load_file(argv[1], &rom_size);
    if (rom_data == NULL)
    {
        perror("Could not load rom file");
        return 1;
    }
    struct sfc_rom rom;
    if (!sfc_load_rom(rom_data, rom_size, SFC_MAP_LO, SFC_CPY_SMART, &rom))
    {
        perror("Could not load rom");
        free(rom_data);
        return 1;
    }

    printf("Checksum: %.4X\n", sfc_checksum(&rom));

    sfc_header *const header = sfc_rom_header(&rom);

    char title[SFC_MAX_TITLE_LENGTH + 1];
    if (!sfc_header_title(header, title))
    {
        perror("Could not get rom title");
        goto error;
    }

    printf("Current title: %s\n", title);

    const char new_title[] = "Test World";
    printf("Setting to: %s\n", new_title);
    if (!sfc_header_set_title(header, new_title))
    {
        perror("Could not set rom title");
        goto error;
    }
    if (!sfc_header_title(header, title))
    {
        perror("Could not get rom title");
        goto error;
    }

    printf("New title: %s\n", title);

    sfc_header_set_speed(header, SFC_SPD_FAST);
    // err_boolean(sfc_header_set_map(header, SFC_MAP_EX_HI), "Failed to write map");

    printf("Speed: %s\n", sfc_header_speed(header) == SFC_SPD_FAST ? "fast" : "slow");

    if (!sfc_save_rom(&rom, argv[2]))
    {
        perror("Could not save rom");
        goto error;
    }
    if (!sfc_header_set_chipset(header, SFC_CHP_RAM | SFC_CHP_BATTERY))
    {
        perror("Could not set chipset");
        goto error;
    }
    enum sfc_chipset const chipset = sfc_header_chipset(header);
    if (chipset == -1)
    {
        perror("Failed to get chipset");
        goto error;
    }

    printf("Has coprocessor: %s, ram: %s, battery: %s\n", (chipset & SFC_CHP_COPROCESSOR) != 0 ? "yes" : "no", (chipset & SFC_CHP_RAM) != 0 ? "yes" : "no", (chipset & SFC_CHP_BATTERY) != 0 ? "yes" : "no");

    printf("Index: %d, Rom size: %d\n", SFC_HDR_ROM_SIZE(header), sfc_header_rom_size(header));
    if (!sfc_header_set_rom_size(header, 1024))
    {
        perror("Could not set rom size");
        goto error;
    }

    printf("Index: %d, Rom size: %d\n", SFC_HDR_ROM_SIZE(header), sfc_header_rom_size(header));

    printf("Index: %d, Ram size: %d\n", SFC_HDR_ROM_SIZE(header), sfc_header_ram_size(header));
    if (!sfc_header_set_ram_size(header, 1024))
    {
        perror("Could not set ram size");
        goto error;
    }
    printf("Index: %d, Ram size: %d\n", SFC_HDR_ROM_SIZE(header), sfc_header_ram_size(header));

    printf("Country: %d\n", sfc_header_country(header));

    printf("Version: %d\n", sfc_header_version(header));

    sfc_header_set_version(header, 2);
    printf("Version: %d\n", sfc_header_version(header));

    sfc_header_set_checksum(header, sfc_checksum(&rom));

    if (!sfc_save_rom(&rom, argv[2]))
    {
        perror("Could not save rom");
        goto error;
    };

    sfc_unload_rom(&rom);
    return 0;

error:
    sfc_unload_rom(&rom);
    return 1;
}
