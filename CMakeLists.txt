cmake_minimum_required(VERSION 3.30)
project(cscript C)

add_compile_definitions("DEBUG=$<CONFIG:Debug>")

set(CMAKE_C_STANDARD 23)

add_executable(cscript cscript.c
        cache.c
        cache.h
        tools.c
        tools.h
        sha256.c
        sha256.h
        script_file.c
        script_file.h
        script_file_type.h
)

install(TARGETS cscript DESTINATION bin)
