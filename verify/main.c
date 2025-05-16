#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <sfc.h>

int main(const size_t argc, const char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No rom specified\n");
        return 2;
    }
    struct sfc_rom rom;
    if (!sfc_load_rom(argv[1], SFC_MAP_HI, false, &rom)) {
        fprintf(stderr, "Failed to load rom: %s", strerror(errno));
        return 2;
    }
    auto const real_sum = sfc_checksum(&rom);
    printf("Real Checksum: %.4X\n", real_sum);

    auto const header = sfc_rom_header(&rom);
    if (header == nullptr) {
        fprintf(stderr, "Failed to get header: %s", strerror(errno));
        return 2;
    }
    auto const expected_sum = sfc_header_checksum(header);
    printf("Expected Checksum: %.4X\n", expected_sum);
    char title[max_title_length + 1];
    if (sfc_header_title(header, title)) {
        printf("Title: %s\n", title);
    } else {
        fprintf(stderr, "Title Corrupt\n");
    }

    if (real_sum != expected_sum) {
        printf("Mismatch!\n");
        sfc_unload_rom(&rom);
        return 1;
    }
    printf("Match!");
    sfc_unload_rom(&rom);
    return 0;
}
