cmake_minimum_required(VERSION 3.31)

project(libsfc
        VERSION 0.1.0
        LANGUAGES C
)

option(BUILD_TESTS "Build CTest tests" OFF)

if (BUILD_TESTS)
    enable_testing()
endif ()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Wswitch-enum -Wswitch-default -Wcast-qual)
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
        src/checksum.c
)
target_include_directories(sfc PUBLIC include/)

install(TARGETS sfc)

add_subdirectory(example)
add_subdirectory(verify)
add_subdirectory(mangle)
if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()
