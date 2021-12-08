file(GLOB_RECURSE sources "*.cpp" "*.h")
add_library(symex ${sources})

generic_includes(symex)

target_link_libraries(symex util)
