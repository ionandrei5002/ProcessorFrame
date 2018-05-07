cmake_minimum_required(VERSION 2.8)

project(ProcessorFrame)

set(HEADERS row.h bytebuffer.h types.h schema.h visitor.h csvreader.h allocator/pool_interface.h allocator/free_list_pool.h allocator/free_list_allocator.h comparator.h)
set(SOURCES ${HEADERS} row.cpp bytebuffer.cpp schema.cpp visitor.cpp csvreader.cpp main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

target_link_libraries(${PROJECT_NAME}
#    tcmalloc
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
    -Wfloat-equal
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
