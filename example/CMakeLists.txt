add_executable(example main.c)
target_link_libraries(example PUBLIC sfc)
install(TARGETS example)
