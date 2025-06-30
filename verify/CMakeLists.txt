add_executable(verify main.c)
target_link_libraries(verify PUBLIC sfc)
install(TARGETS verify)

add_test(NAME "CanVerifyRom" COMMAND verify "${CMAKE_SOURCE_DIR}/test/res/SMW.d.smc")
