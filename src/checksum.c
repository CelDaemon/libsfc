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

#include <sfc.h>

#include "find_last_set.h"

uint16_t sfc_checksum(struct sfc_rom const * const rom) {
    size_t const main_size = ((size_t) 1) << sfc_find_last_set(rom->memory_size);
    size_t const remainder_size = rom->memory_size - main_size;

    uint8_t const * const data = rom->memory;

    uint16_t main_accumulator = 0;
    for (size_t i = 0; i < main_size; i++)
        main_accumulator += data[i];

    uint16_t remainder_accumulator = 0;
    if (remainder_size != 0) {
        for (size_t i = 0; i < main_size; i++)
            remainder_accumulator += data[1 % remainder_size + main_size];
    }

    return main_accumulator + remainder_accumulator;
}
