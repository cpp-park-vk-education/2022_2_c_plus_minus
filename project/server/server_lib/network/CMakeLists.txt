cmake_minimum_required(VERSION 3.0)
project(network)
set(CMAKE_CXX_STANDARD 20)

OPTION(BUILD_TEST "TEST" OFF)
OPTION(BUILD_DEV  "DEVELOPMENT BUILD" ON)
OPTION(BUILD_VALGRIND "MEMORY TESTING" OFF)
OPTION(BUILD_SANITIZE "SANITIZE" OFF)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -pedantic -Wformat=2 -Wfloat-equal -Wconversion")
string(APPEND CMAKE_CXX_FLAGS " -Wlogical-op  -Wduplicated-cond -Wcast-qual -Wcast-align")

if(BUILD_DEV)
    string(APPEND CMAKE_CXX_FLAGS " -g -O0")
#    add_subdirectory(tests)
endif()

if(BUILD_SANITIZE)
    string(APPEND CMAKE_CXX_FLAGS " -fsanitize=address,undefined -fno-sanitize-recover=all -fsanitize-undefined-trap-on-error")
endif()

if(BUILD_TEST)
    add_subdirectory(tests)
endif()

# Collects the names of all the source files in the specified directory
aux_source_directory(server/src SERVER_SRC)
aux_source_directory(room/src ROOM_SRC)
aux_source_directory(connection/src CONN_SRC)
aux_source_directory(handlers/src HANDLERS_SRC)
aux_source_directory(channel/src CHANNEL_SRC)
aux_source_directory(router/src ROUTER_SRC)

add_library(${PROJECT_NAME} ${SERVER_SRC} ${ROOM_SRC} ${CONN_SRC}
            ${HANDLERS_SRC} ${SESSION_SRC} ${CHANNEL_SRC} ${ROUTER_SRC})

find_package(Boost REQUIRED COMPONENTS system thread json)
include_directories(${Boost_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC
        connection/include
        handlers/include/interface handlers/include/impl
        room/include/interface room/include/impl
        server/include/interface server/include/impl
        router/include/
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC general_lib game_logic ${Boost_LIBRARIES}
        )
