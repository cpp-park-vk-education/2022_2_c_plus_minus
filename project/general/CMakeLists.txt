cmake_minimum_required(VERSION 3.0)
project(general_lib)
set(CMAKE_CXX_STANDARD 20)

message("BUILD GENERAL SOURCES")

aux_source_directory(user/src USER_SRC)

find_package(Boost REQUIRED system log log_setup json)
include_directories(${Boost_INCLUDE_DIRS})

add_library(${PROJECT_NAME} STATIC ${USER_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC  message/include
        user/include safe_queue/include write_buffer/include)

target_link_libraries(${PROJECT_NAME})