/*
    Copyright 2025 CelDaemon

    Redistribution and use in source and binary forms, with or without modification,are permitted provided
    that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
    the following disclaimer in the documentation and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or
    promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
    IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <sfc.h>

#ifdef _MSC_VER
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

char const *parse_program_name(char const *path) {
    if (path == NULL)
        return "verify";
    char const *name = strrchr(path, PATH_SEPARATOR);
    if (name == NULL)
        return path;
    return name + 1;
}
static char const *program_name;

static void print_usage(void) {
    fprintf(stderr, "Usage: %s [OPTION...] FILE\n", program_name);
}

int main(int const argc, char *argv[])
{
    if (argc < 2) {
        print_usage();
        return 1;
    }
    struct sfc_rom * const rom = sfc_read_rom(argv[1], NULL, NULL);
    if (rom == NULL) {
        fprintf(stderr, "%s: Failed to load ROM\n", program_name);
        return 1;
    }
    struct sfc_header const * const header = sfc_rom_header(rom);
    char title[SFC_HEADER_TITLE_MAX_SIZE + 1];
    sfc_header_title(header, title);
    printf("ROM title: %s\n", title);
    uint16_t const expected_checksum = sfc_header_checksum(header);
    printf("Expected checksum: %" PRIX16 "\n", expected_checksum);
    uint16_t const calculated_checksum = sfc_checksum(rom);
    printf("Calculated checksum: %" PRIX16 "\n", calculated_checksum);


    sfc_destroy_rom(rom);
    if (calculated_checksum == expected_checksum) {
        printf("Match!");
        return 0;
    }
    printf("Mismatch!");
    return 1;
}
