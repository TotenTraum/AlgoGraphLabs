cmake_minimum_required(VERSION 3.21)
project(lab7)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(lab7 main.cpp)
target_link_libraries(lab7 GRAPH)
