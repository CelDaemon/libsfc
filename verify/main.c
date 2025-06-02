#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <sfc/rom.h>
#include <sfc/checksum.h>
#include <sfc/header.h>

#ifdef _WIN32
#  define DIR_SEP '\\'
#  define STRERROR(err, buf, buf_len) strerror_s(buf, buf_len, err)
#else
#  define DIR_SEP '/'
#  define STRERROR(err, buf, buf_len) strerror_r(err, buf, buf_len)
#endif

int main(const int argc, const char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No rom specified\n");
        const char* name = strrchr(argv[0], DIR_SEP);
        if (name != NULL)
            name++;
        else
            name = argv[0];
        fprintf(stderr, "Usage: %s [FILE]\n", name);
        return 2;
    }
    size_t size;
    void *const rom_data = sfc_load_file(argv[1], &size);
    if (rom_data == NULL)
    {
        perror("Failed to load rom file");
        return 2;
    }
    struct sfc_rom rom;
    if (!sfc_load_rom(rom_data, size, SFC_MAP_HI, SFC_CPY_SMART, &rom)) {
        perror("Failed to load rom");
        return 2;
    }
    uint16_t const real_sum = sfc_checksum(&rom);
    printf("Real Checksum: %.4X\n", real_sum);

    const sfc_header *header = sfc_rom_header(&rom);
    if (header == NULL) {
        perror("Failed to get header");
        return 2;
    }
    uint16_t const expected_sum = sfc_header_checksum(header);
    printf("Expected Checksum: %.4X\n", expected_sum);
    char title[SFC_MAX_TITLE_LENGTH + 1];
    if (sfc_header_title(header, title)) {
        printf("Title: %s\n", title);
    } else {
        perror("Title corrupt");
    }

    if (real_sum != expected_sum) {
        printf("Mismatch!\n");
        sfc_unload_rom(&rom);
        return 1;
    }
    printf("Match!\n");
    sfc_unload_rom(&rom);
    return 0;
}
