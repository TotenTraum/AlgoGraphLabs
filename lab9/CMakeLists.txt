cmake_minimum_required(VERSION 3.21)
project(lab9)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(lab9 main.cpp)
target_include_directories(lab9 PUBLIC ${PROJECT_SOURCE_DIR}/include)