cmake_minimum_required(VERSION 3.31)

project(libsfc
        VERSION 0.1.0
        LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Wswitch-enum -Wswitch-default)
endif ()

add_library(sfc STATIC
        include/sfc.h
        src/rom.c
        src/map.h
        src/map.c
        src/header.h
        src/header.c
        src/offset.h
        src/find_last_set.h
        src/find_last_set.c
        src/checksum.c
)
target_include_directories(sfc PUBLIC include/)

add_subdirectory(example)
add_subdirectory(verify)

install(TARGETS sfc example verify)
