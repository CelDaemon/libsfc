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

#include "util.h"
#include "../src/map.h"

#define MARIO_WORLD_SIZE 524800
#define MARIO_WORLD_2_SIZE 2097152
#define PRIMAL_RAGE_SIZE 3145728

#define BOOL_STRING(val) ((val) ? "true" : "false")

static bool check_size(size_t const size, bool const val) {
    bool const copier = sfc_deduce_copier((size));
    if(copier != (val)) {\
        fprintf(stderr, "Size: %zu, Expected: %s, Actual: %s\n", size, BOOL_STRING(val), BOOL_STRING(copier));
        return false;
    }
    return true;
}

int can_deduce_copier(int const argc, char* const argv[]) {
    (void) argc;
    (void) argv;
    ASSERT_TRUE(check_size(MARIO_WORLD_SIZE, true));
    ASSERT_TRUE(check_size(MARIO_WORLD_2_SIZE, false));
    ASSERT_TRUE(check_size(PRIMAL_RAGE_SIZE, false));
    return 0;
}
