cmake_minimum_required(VERSION 3.31)
project(libsfc C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)


add_library(sfc STATIC
        include/sfc.h
        src/rom.c
        src/mapping.h
        src/header.c
        src/checksum.c
        src/mapping.c
)
target_include_directories(sfc PUBLIC include/)
if (MSVC)
    target_compile_options(sfc PRIVATE /Wall)
else ()
    target_compile_options(sfc PRIVATE -Wall -Wextra -Wpedantic)
endif ()
add_subdirectory(example)
add_subdirectory(verify)