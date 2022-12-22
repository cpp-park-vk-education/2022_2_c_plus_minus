cmake_minimum_required(VERSION 3.0.0)

project(basic_graphics)

set(CMAKE_CXX_FLAGS "-std=c++2a -Wall -Wpedantic -Wextra -lsfml-graphics -lsfml-window -lsfml-system")

add_executable(${PROJECT_NAME} GUIObj.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics sfml-window sfml-system)