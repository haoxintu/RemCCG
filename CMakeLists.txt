cmake_minimum_required(VERSION 3.5)

project(remccg)

set(PROTO_CXX_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROTO_CXX_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# use clang++-6.0 so need to compile libFuzzer.a independentl
set(CMAKE_CXX_COMPILER clang++-6.0)
set(CMAKE_C_COMPILER clang-6.0)
set(CMAKE_CXX_FLAGS "-g -w -fPIC -std=c++11 -pthread")

add_subdirectory(lib/ccg)
add_subdirectory(src)
