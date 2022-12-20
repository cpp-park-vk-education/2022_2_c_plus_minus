cmake_minimum_required(VERSION 3.0)
project(server_lib_tests)
set(CMAKE_CXX_STANDARD 17)

# Compilation flags
string(APPEND CMAKE_CXX_FLAGS " -g -Wpedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -fprofile-arcs -pthread -ftest-coverage -fPIC")
string(APPEND CMAKE_CXX_FLAGS " -O0")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )

enable_testing()
find_package(GTest REQUIRED)

# Collects the names of all the source files in the specified directory
aux_source_directory(. SRC)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} server_lib database_lib GTest::gtest_main)
gtest_discover_tests(${PROJECT_NAME})