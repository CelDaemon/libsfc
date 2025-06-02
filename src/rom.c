#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "sfc/rom.h"

#include "detect.h"
#include "sfc/header.h"

#include "mapping.h"

char *sfc_version = "BOOOO";

#ifdef _MSC_VER
#  define fileno _fileno
#endif

#define CHUNK_SIZE 2048

void *sfc_load_file(const char *path, size_t *size)
{
    assert(path != NULL);
    FILE *const file = fopen(path, "rb");
    if (file == NULL)
        return false;
    struct stat stat;
    if (fstat(fileno(file), &stat) != 0)
        goto error_1;
    size_t current_size = 0;

    void *data = malloc(current_size);
    if (data == NULL)
        goto error_1;

    size_t cursor = 0;
    uint8_t chunk[CHUNK_SIZE];
    size_t chunk_size = 0;
    do
    {
        chunk_size = fread(chunk, sizeof(uint8_t), CHUNK_SIZE, file);

        // Resize the data buffer if exceeding the predicted size
        if (chunk_size > current_size - cursor)
        {
            if (fstat(fileno(file), &stat) != 0)
                goto error_2;
            size_t const min_size = cursor + chunk_size;
            size_t const file_size = stat.st_size;
            size_t const new_size = file_size > min_size ? file_size : min_size;

            void *const new_data = realloc(data, new_size);
            if (new_data == NULL)
                goto error_2;
            current_size = new_size;
            data = new_data;
        }

        memcpy(((uint8_t*)data) + cursor, chunk, chunk_size);
        cursor += chunk_size;

    } while (chunk_size == CHUNK_SIZE);

    if (ferror(file))
        goto error_2;
    if (fclose(file) == EOF)
        goto error_2;

    *size = current_size;

    return data;
error_2:
    free(data);
error_1:
    fclose(file);
    return NULL;
}


bool sfc_load_rom(void *data, const size_t size, const enum sfc_mapping map, const enum sfc_copier copier, struct sfc_rom *rom) {
    assert(data != NULL);
    assert(SFC_MAP_CONCRETE(map));
    assert(rom != NULL);

    if (size <= sfc_data_offset(copier) + sfc_header_offset(map) + SFC_HDR_SIZE) {
        errno = EINVAL;
        return false;
    }

    // TODO Infer the mapping based on ROM contents
    bool copier_bool;
    switch (copier)
    {
    case SFC_CPY_SMART:
        copier_bool = sfc_detect_copier(size);
        break;
    case SFC_CPY_FALSE:
        copier_bool = false;
        break;
    case SFC_CPY_TRUE:
        copier_bool = true;
        break;
    default:
        errno = EINVAL;
        return false;
    }


    const struct sfc_rom new_rom = {
        .data = data,
        .size = size,
        .map = map,
        .copier = copier_bool
    };
    *rom = new_rom;

    return true;
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
