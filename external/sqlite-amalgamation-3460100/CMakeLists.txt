add_lib(sqlite3 sqlite3.c)

# Some GCC versions are paranoid
target_compile_options(sqlite3 PRIVATE -Wno-cast-function-type -Wno-implicit-fallthrough)
target_link_libraries(sqlite3 PUBLIC dl pthread)
