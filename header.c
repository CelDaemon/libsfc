#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "header.h"
#include "mapping.h"


static size_t title_length(const char title[static max_title_length]) {
    size_t length = 0;
    for (int i = 0; i < max_title_length; i++) {
        if (title[max_title_length - i - 1] == ' ')
            continue;

        length = max_title_length - i;
        break;
    }
    return length;
}

static bool valid_title(const char *title, const size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (isprint(title[i]))
            continue;
        return false;
    }
    return true;
}

bool sfc_rom_title(const struct sfc_rom *rom, char title[static max_title_length + 1]) {
    auto const header = rom_header(rom);
    if (!valid_title(header->title, max_title_length)) {
        errno = EINVAL;
        return false;
    }
    memcpy(title, header->title, max_title_length);
    title[title_length(title)] = '\0';
    return true;
}

bool sfc_set_rom_title(const struct sfc_rom *rom, const char *title) {
    auto const length = strlen(title);
    if (length > max_title_length || !valid_title(title, length)) {
        errno = EINVAL;
        return false;
    }
    auto const header = rom_header(rom);
    memcpy(header->title, title, length);
    memset(header->title + length, ' ', max_title_length - length);
    return true;
}