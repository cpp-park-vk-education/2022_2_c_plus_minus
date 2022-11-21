cmake_minimum_required(VERSION 3.0)
project(server_lib)
set(CMAKE_CXX_STANDARD 20)
add_subdirectory(game_logic)
add_subdirectory(network)
add_subdirectory(tests)

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE network game_logic)
