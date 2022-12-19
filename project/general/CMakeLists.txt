cmake_minimum_required(VERSION 3.0)
project(general_lib)
set(CMAKE_CXX_STANDARD 20)

message("BUILD GENERAL SOURCES")

aux_source_directory(user/src USER_SRC)
aux_source_directory(buffer/src BUFFER_SRC)
aux_source_directory(message/src MSG_SRC)

find_package(Boost REQUIRED system log log_setup json)
include_directories(${Boost_INCLUDE_DIRS})

add_library(${PROJECT_NAME} STATIC ${USER_SRC} ${BUFFER_SRC} ${MSG_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC  message/include
        user/include safe_queue/include buffer/include logger/include)

target_link_libraries(${PROJECT_NAME} server_lib)