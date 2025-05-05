#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sfc.h"

#include "../header.h"

int main() {
    printf("%zX", offsetof(struct sfc_header, checksum) + 0xB0);
    struct sfc_rom rom;
    if (!sfc_load_rom("../../rom.smc", SFC_MAP_LO, false, &rom)) {
        fprintf(stderr, "%s", strerror(errno));
        exit(1);
    }

    sfc_unload_rom(&rom);
}

