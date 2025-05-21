#include "sfc.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "mapping.h"

#include "header.h"

#ifdef _MSC_VER
#  define fileno _fileno
#endif

bool sfc_load_rom(const char *path, const enum sfc_map map, const bool copier, struct sfc_rom *rom) {
    assert(path != NULL);
    assert(SFC_MAP_CONCRETE(map));
    assert(rom != NULL);


    FILE *const file = fopen(path, "rb");

    if (file == NULL)
        return false;
    struct stat stat;
    if (fstat(fileno(file), &stat) != 0)
        goto error_1;
    size_t size = stat.st_size;

    void *data = malloc(size); // Allocate the buffer with the predicted size
    if (data == NULL)
        goto error_1;

    size_t offset = 0;
    uint8_t chunk[2048];
    size_t chunk_size = 0;
    do {
        chunk_size = fread(chunk, sizeof(uint8_t), 2048, file);

        // Resize the data buffer if exceeding the predicted size
        if (chunk_size > size - offset) {
            size_t const new_size = offset + chunk_size;
            void *const new_data = realloc(data, new_size);
            if (new_data == NULL)
                goto error_2;
            size = new_size;
            data = new_data;
        }

        memcpy(((uint8_t*)data) + offset, chunk, chunk_size);
        offset += chunk_size;
    } while (chunk_size == 2048);

    if (ferror(file))
        goto error_2;

    if (size <= sfc_data_offset(copier) + sfc_header_offset(map) + SFC_HDR_SIZE) {
        errno = EINVAL;
        goto error_2;
    }

    // TODO Infer the mapping based on ROM contents


    if (fclose(file) == EOF)
        goto error_2;


    const struct sfc_rom new_rom = {
        .data = data,
        .size = size,
        .map = map,
        .copier = copier
    };
    *rom = new_rom;

    return true;

error_2:
    free(data);
error_1:
    fclose(file);
    return false;
}

bool sfc_save_rom(const struct sfc_rom *rom, const char *path) {
    assert(rom != NULL);
    assert(path != NULL);

    FILE *const file = fopen(path, "wb");

    if (file == NULL)
        return false;

    if (!fwrite(rom->data, rom->size, 1, file))
        goto error_1;

    if (fclose(file) == EOF)
        goto error_1;

    return true;

error_1:
    fclose(file);

    return false;
}

void sfc_unload_rom(struct sfc_rom *rom) {
    assert(rom != NULL);
    free(rom->data);
    rom->data = NULL;
}