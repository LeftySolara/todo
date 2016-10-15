cmake_minimum_required(VERSION 3.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/test)

file(GLOB SOURCES "./*.cpp")
 
add_executable(todo_tests ${SOURCES})
