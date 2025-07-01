set(TEST_SOURCES
        CanReadRom.c
        CanIntrospectCopier.c
)

create_test_sourcelist(TESTS main.c ${TEST_SOURCES})

add_executable(tests ${TESTS})
target_link_libraries(tests PRIVATE sfc)
target_compile_definitions(tests PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

foreach (TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME "${TEST_SOURCE}" NAME_WE)
    add_test(NAME "${TEST_NAME}" COMMAND tests "${TEST_NAME}")
endforeach ()
