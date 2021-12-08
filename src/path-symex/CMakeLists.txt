file(GLOB_RECURSE sources "*.cpp" "*.h")
add_library(path-symex ${sources})

generic_includes(path-symex)

target_link_libraries(path-symex util)
