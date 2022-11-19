cmake_minimum_required(VERSION 3.0)
project(FunChess)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTS "Build tests" OFF)

add_subdirectory(project)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()


