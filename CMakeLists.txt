cmake_minimum_required(VERSION 3.0)
project(todo)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
 
# include_directories(include)
file(GLOB SOURCES "src/*.cpp")
 
add_executable(todo ${SOURCES})
