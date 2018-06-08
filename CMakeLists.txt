cmake_minimum_required(VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_SOURCE_DIR}/tests/)

enable_testing ()

include(${PROJECT_SOURCE_DIR}/src/source/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/src/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt)