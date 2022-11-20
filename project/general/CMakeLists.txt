cmake_minimum_required(VERSION 3.0)
project(general)

set(CMAKE_CXX_STANDARD 20)

file(GLOB ${PROJECT_NAME}_src user/src/*.cpp)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_src})

target_include_directories(${PROJECT_NAME} PUBLIC user/include)
