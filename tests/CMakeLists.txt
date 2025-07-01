create_test_sourcelist(TESTS main.c
        can_read_rom.c
        can_deduce_copier.c
)

add_executable(tests ${TESTS})
target_link_libraries(tests PRIVATE sfc)
target_compile_definitions(tests PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_test(NAME "Can read rom" COMMAND tests can_read_rom)
add_test(NAME "Can deduce copier" COMMAND tests can_deduce_copier)
