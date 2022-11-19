PROJECT(chess_client_server)
cmake_minimum_required(VERSION 3.0)

OPTION(BUILD_SERVER "Build server" OFF)
OPTION(BUILD_CLIENT "Build client" OFF)
OPTION(BUILD_ALL    "Build all"    OFF)

if((BUILD_SERVER) OR (BUILD_ALL))
    message("BUILD SERVER")
    add_subdirectory(server)
endif()

if((BUILD_CLIENT) OR (BUILD_ALL))
    message("BUILD CLIENT")
    add_subdirectory(client)
endif()

