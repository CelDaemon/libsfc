cmake_minimum_required(VERSION 3.31)


option(CI_BUILD "Whether this build is part of CI" OFF)
option(CI_BUILD_TYPE "What type of build to produce" "Debug")


project(libsfc
        VERSION 0.1.0
        LANGUAGES C)


set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)



if (MSVC)
    add_compile_options(/W4)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

if (CI_OUTPUT)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_BINARY_DIR}/bin>")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<1:${CMAKE_BINARY_DIR}/lib>")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "$<1:${CMAKE_BINARY_DIR}/lib>")
endif ()

add_library(sfc STATIC
        src/rom.c include/sfc/rom.h
        src/checksum.c include/sfc/checksum.h
        src/mapping.c src/mapping.h
        src/header.c include/sfc/header.h
        src/last_bit.c src/last_bit.h
        src/detect.c src/detect.h
)
target_include_directories(sfc PUBLIC include/)
add_subdirectory(example)
add_subdirectory(verify)
