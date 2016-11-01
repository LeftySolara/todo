cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories("../src")

file(GLOB TESTS "./*.cpp")
file(GLOB SOURCES "../src/*.cpp")

add_executable(todo_tests ${TESTS} ${SOURCES})
target_link_libraries(todo_tests -lsqlite3)
