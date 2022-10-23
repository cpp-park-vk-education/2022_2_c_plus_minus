PROJECT(chess_client_server)
cmake_minimum_required(VERSION 3.0)

OPTION(BUILD_SERVER "Build server" OFF)
OPTION(BUILD_CLIENT "Build client" OFF)
OPTION(BUILD_ALL    "Build all"    ON)

if((BUILD_SERVER) OR (BUILD_ALL))
    add_subdirectory(server)
endif()

if((BUILD_CLIENT) OR (BUILD_ALL))
    add_subdirectory(client)
endif()