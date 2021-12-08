file(GLOB_RECURSE sources "*.cpp" "*.h")
add_library(aa-path-symex ${sources})

generic_includes(aa-path-symex)

target_link_libraries(aa-path-symex util)
