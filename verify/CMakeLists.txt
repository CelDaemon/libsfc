add_executable(verify main.c)
target_link_libraries(verify PUBLIC sfc)

if (BUILD_TESTS)
    add_test(NAME "Can verify rom" COMMAND verify "${CMAKE_SOURCE_DIR}/tests/SMW.d.smc")
endif ()
