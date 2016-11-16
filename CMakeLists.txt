cmake_minimum_required(VERSION 3.0)
project(todo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
 
file(GLOB sources "src/*.cpp")
 
add_executable(todo ${sources})
target_link_libraries(todo -lsqlite3)
