cmake_minimum_required(VERSION 3.31)
project(libsfc C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)


add_library(sfc STATIC
        public/sfc.h
        rom.c
        mapping.h
        header.c
)
target_include_directories(sfc PUBLIC public/)
target_compile_options(sfc PRIVATE -Wall -Wextra -Wpedantic)
add_subdirectory(example)