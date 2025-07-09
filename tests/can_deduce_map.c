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

#include "util.h"
#include "../src/map.h"




static char const *map_string(enum sfc_map const map) {
    switch (map) {
        case SFC_MAP_LO:
            return "lo";
        case SFC_MAP_HI:
            return "hi";
        case SFC_MAP_EX_HI:
            return "ex_hi";
        default:
            return "?";
    }
}

static bool check_map(char const * const path, enum sfc_map const val) {
    size_t size;
    void * const data = read_file(path, &size);
    if(data == NULL) {\
        fprintf(stderr, "Failed to open file: %s", path);
        return false;
    }
    bool const map = sfc_deduce_map(data, size);
    free(data);
    if(map != (val)) {
        fprintf(stderr, "Size: %zu, Expected: %s, Actual: %s\n", size, map_string(val), map_string(map));
        return false;
    }
    return true;
}


int can_deduce_map(int const argc, char * const argv[]) {
    (void) argc;
    (void) argv;
    ASSERT_TRUE(check_map(RESOURCE("SMW.d.smc"), SFC_MAP_LO));
    ASSERT_TRUE(check_map(RESOURCE("SMW2.d.smc"), SFC_MAP_LO));
    ASSERT_TRUE(check_map(RESOURCE("PRAGE.d.smc"), SFC_MAP_HI));
    ASSERT_TRUE(check_map(RESOURCE("SD3.d.smc"), SFC_MAP_HI));
    return 0;
}
