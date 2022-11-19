cmake_minimum_required(VERSION 3.0)
project(server)
set(CMAKE_CXX_STANDARD 17)

OPTION(BUILD_TEST "TEST" OFF)
OPTION(BUILD_DEV  "DEVELOPMENT BUILD" ON)
OPTION(BUILD_VALGRIND "MEMORY TESTING" OFF)
OPTION(BUILD_SANITIZE "SANITIZE" OFF)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -pedantic -Wformat=2 -Wfloat-equal -Wconversion -fprofile-arcs")
string(APPEND CMAKE_CXX_FLAGS " -Wlogical-op  -Wduplicated-cond -Wcast-qual -Wcast-align")

if(BUILD_DEV)
    string(APPEND CMAKE_CXX_FLAGS " -g -O0")
    add_subdirectory(tests)
endif()

if(BUILD_SANITIZE)
    string(APPEND CMAKE_CXX_FLAGS " -fsanitize=address,undefined -fno-sanitize-recover=all -fsanitize-undefined-trap-on-error")
endif()

if(BUILD_TEST)
    add_subdirectory(tests)
endif()

# Collects the names of all the source files in the specified directory
aux_source_directory(src SRC)
add_library(${PROJECT_NAME}_lib ${SRC})
target_include_directories(${PROJECT_NAME}_lib PUBLIC include)

add_executable(${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        )

target_link_libraries(${PROJECT_NAME}_lib

        )

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)