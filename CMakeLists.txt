cmake_minimum_required(VERSION 3.31)
project(libsfc C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)


add_library(sfc STATIC
        sfc.c
        public/sfc.h
        mapping.h
        header.h
)
target_include_directories(sfc PUBLIC public/)
target_compile_options(sfc PRIVATE -Wall -Wextra)
add_subdirectory(example)