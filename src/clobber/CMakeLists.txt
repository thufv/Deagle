file(GLOB_RECURSE sources "*.cpp" "*.h")
add_library(clobber ${sources})

generic_includes(clobber)

target_link_libraries(clobber util)
