cmake_minimum_required(VERSION 3.0)
project(database_lib)
set(CMAKE_CXX_STANDARD 20)

message("BUILD DB_LIB")

set(IncludeDirs ${CMAKE_CURRENT_SOURCE_DIR}/include/impl/
        ${CMAKE_CURRENT_SOURCE_DIR}/include/interface/
        ${CMAKE_CURRENT_SOURCE_DIR}/src/)

find_library(PQXX_LIB pqxx REQUIRED)
find_library(PQ_LIB pq REQUIRED)

# Collects the names of all the source files in the specified directory
aux_source_directory(src SRC)

add_library(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ${IncludeDirs})
target_link_libraries(${PROJECT_NAME} general ${PQ_LIB} ${PQXX_LIB})