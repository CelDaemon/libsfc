add_executable(mangle main.c)
target_link_libraries(mangle PUBLIC sfc)
install(TARGETS mangle)
