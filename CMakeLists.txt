cmake_minimum_required(VERSION 3.31)


option(CI_BUILD "Whether this build is part of CI" OFF)
option(CI_BUILD_TYPE "What type of build to produce" "Debug")


project(libsfc C)




set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

if (CI_BUILD)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_BINARY_DIR}/bin>")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<1:${CMAKE_BINARY_DIR}/lib>")
    set(CMAKE_BUILD_TYPE "${CI_BUILD_TYPE}")
    set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
endif ()

add_library(sfc STATIC
        src/rom.c include/sfc/rom.h
        src/checksum.c include/sfc/checksum.h
        src/mapping.c src/mapping.h
        src/header.c include/sfc/header.h
        src/last_bit.c src/last_bit.h
)
target_include_directories(sfc PUBLIC include/)
if (MSVC)
    target_compile_options(sfc PRIVATE /W4)
else ()
    target_compile_options(sfc PRIVATE -Wall -Wextra -Wpedantic)
endif ()
add_subdirectory(example)
add_subdirectory(verify)
