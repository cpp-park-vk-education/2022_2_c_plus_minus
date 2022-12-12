cmake_minimum_required(VERSION 3.0)
project(server)
set(CMAKE_CXX_STANDARD 20)
add_subdirectory(server_lib)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} server_lib)
