#include "sfc.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mapping.h"

/**
 * @return The file size
 * @attention Seeks to end of file
 */
static size_t file_size(FILE * const file) {
    assert(file != nullptr);
    fseek(file, 0, SEEK_END);
    return ftell(file);
}

bool sfc_load_rom(const char *path, const enum sfc_map map, const bool copier, struct sfc_rom *rom) {
    assert(path != nullptr);
    assert(SFC_MAP_CONCRETE(map));
    assert(rom != nullptr);


    auto const file = fopen(path, "rb");

    if (file == nullptr)
        return false;

    auto size = file_size(file);
    rewind(file); // Rewind the file after getting its size

    void *data = malloc(size); // Allocate the buffer with the predicted size
    if (data == nullptr)
        goto error_1;

    constexpr auto max_chunk_size = 2048; // Read in chunks of 2KiB
    size_t offset = 0;
    uint8_t chunk[max_chunk_size];
    size_t chunk_size = 0;
    do {
        chunk_size = fread(chunk, sizeof(uint8_t), max_chunk_size, file);

        // Resize the data buffer if exceeding the predicted size
        if (chunk_size > size - offset) {
            auto const new_size = offset + chunk_size;
            auto const new_data = realloc(data, new_size);
            if (new_data == nullptr)
                goto error_2;
            size = new_size;
            data = new_data;
        }

        memcpy(data + offset, chunk, chunk_size);
        offset += chunk_size;
    } while (chunk_size == max_chunk_size);

    if (ferror(file))
        goto error_2;

    if (size <= data_offset(copier) + header_offset(map) + sizeof(struct sfc_header)) {
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
    assert(rom != nullptr);
    assert(path != nullptr);

    auto const file = fopen(path, "wb");

    if (file == nullptr)
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
    assert(rom != nullptr);
    free(rom->data);
    rom->data = nullptr;
}