add_executable(verify main.c)
target_link_libraries(verify PUBLIC sfc)
install(TARGETS verify)

if (BUILD_TESTING)
    add_test(NAME "CanVerifyRom" COMMAND verify "${CMAKE_SOURCE_DIR}/tests/SMW.d.smc")
endif ()
