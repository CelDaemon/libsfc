set(TEST_SOURCES
        CanReadRom.c
        CanIntrospectCopier.c
)

set(TEST_RESOURCES
        SMW.d.smc
)

create_test_sourcelist(TESTS main.c ${TEST_SOURCES})

add_executable(tests ${TESTS})
target_link_libraries(tests PRIVATE sfc)

foreach (TEST_RESOURCE ${TEST_RESOURCES})
    add_custom_command(TARGET tests POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_RESOURCE}" "$<TARGET_FILE_DIR:tests>/${TEST_RESOURCE}")
endforeach ()

foreach (TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME "${TEST_SOURCE}" NAME_WE)
    add_test(NAME "${TEST_NAME}" COMMAND tests "${TEST_NAME}")
endforeach ()
