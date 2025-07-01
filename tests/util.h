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

#ifndef _SFC_TEST_UTIL_H
#define _SFC_TEST_UTIL_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <sfc.h>

#ifdef _MSC_VER
#define FILENO _fileno
#else
#define FILENO fileno
#endif

#define ASSERT_TRUE(value) if(!(value))\
    return 1;\

#define RESOURCE(path) RESOURCE_DIR "/" path


static inline void *read_file(char const *path, size_t *size)  {
    FILE * const file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return NULL;
    }
    struct stat stat;
    if (fstat(FILENO(file), &stat) != 0) {
        fprintf(stderr, "Failed to get file stats\n");
        fclose(file);
        return NULL;
    }
    void * const data = malloc(stat.st_size);
    if (fread(data, stat.st_size, 1, file) != 1) {
        fprintf(stderr, "Failed to read file data\n");
        fclose(file);
        free(data);
        return NULL;
    }
    fclose(file);
    *size = stat.st_size;
    return data;
}

#endif
