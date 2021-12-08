file(GLOB_RECURSE sources "*.cpp" "*.h")
add_library(aa-symex ${sources})

generic_includes(aa-symex)

target_link_libraries(aa-symex util)
