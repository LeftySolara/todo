cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
get_filename_component(SRC_DIR "../src" ABSOLUTE)

include_directories(${SRC_DIR})

file(GLOB tests ${CMAKE_SOURCE_DIR}/*.cpp)
file(GLOB sources ${SRC_DIR}/*.cpp)

# Don't include file with main(), since Catch provides one for us
list(REMOVE_ITEM sources ${SRC_DIR}/todo.cpp)

add_executable(todo_tests ${tests} ${sources})
target_link_libraries(todo_tests -lsqlite3)
