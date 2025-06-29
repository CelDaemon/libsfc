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

#include <getopt.h>

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

struct option long_opts[] = {
    {"verbose", no_argument, NULL, 'v'},
    {NULL, no_argument, NULL, 0}
};

static char const *program_name;

static void print_usage(void) {
    fprintf(stderr, "Usage: %s [OPTION...] FILE\n", program_name);
}

int main(int const argc, char *argv[])
{
    program_name = parse_program_name(argv[0]);
    bool verbose = false;
    int opt;
    while ((opt = getopt_long(argc, argv, "v", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'v':
                verbose = true;
                break;
            default:
                break;
        }
    }
    if (optind >= argc) {
        print_usage();
        return 1;
    }
    struct sfc_rom * const rom = sfc_read_rom(argv[optind++], NULL, NULL);
    if (rom == NULL) {
        fprintf(stderr, "%s: Failed to load ROM\n", program_name);
        return 1;
    }
    struct sfc_header const * const header = sfc_rom_header(rom);
    if (verbose) {
        char title[SFC_HEADER_TITLE_MAX_SIZE + 1];
        sfc_header_title(header, title);
        fprintf(stderr, "ROM title: %s\n", title);
    }
    uint16_t const expected_checksum = sfc_header_checksum(header);
    if (verbose)
        fprintf(stderr, "Expected checksum: %" PRIX16 "\n", expected_checksum);
    uint16_t const calculated_checksum = sfc_checksum(rom);
    if (verbose)
        fprintf(stderr, "Calculated checksum: %" PRIX16 "\n", calculated_checksum);

    sfc_destroy_rom(rom);
    if (calculated_checksum == expected_checksum) {
        if (verbose)
            fprintf(stderr, "Match!");
        return 0;
    }
    if (verbose)
        fprintf(stderr, "Mismatch!");
    return 1;
}
