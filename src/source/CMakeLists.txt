cmake_minimum_required(VERSION 2.8)

project(ProcessorLib)

include_directories(${PROJECT_SOURCE_DIR}/src/source/)

set(DIR ${CMAKE_CURRENT_LIST_DIR})

file(
    GLOB_RECURSE 
    SRC
    ${DIR}/*.cpp
)
add_library(${PROJECT_NAME} 
    SHARED 
    ${SRC}
)

target_link_libraries(${PROJECT_NAME}
#    tcmalloc
    stdc++fs
)

target_compile_options(${PROJECT_NAME}
  PRIVATE
    -flto
    -march=native
    -std=c++17
#    -O
    -O3
#    -g3
    -m64
    -fno-omit-frame-pointer
    -Werror
    -Wno-unused-parameter
    -Wfatal-errors
    -pedantic-errors
    -Wall
    -Wno-missing-braces
    -Wcast-align
    -Wcast-qual
    -Wctor-dtor-privacy
    -Wextra
    -Wformat=2
    -Wmissing-declarations
    -Wno-long-long
    -Wnon-virtual-dtor
    -Woverlength-strings
    -Woverloaded-virtual
    -Wredundant-decls
    -Wreorder
    -Wswitch-default
    -Wunused-variable
    -Wwrite-strings
    -Wno-variadic-macros
    -Wno-unused-local-typedef
    -Wno-language-extension-token
    -fstrict-aliasing
    -Wconversion
#    -Wfloat-equal
    -Wold-style-cast
    -Wunreachable-code
    -Wshadow
    -Wpointer-arith
    -Wcast-align
    -Wformat-nonliteral
    -Winit-self
    -floop-parallelize-all
    -ftree-parallelize-loops=4
)
