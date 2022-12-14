cmake_minimum_required(VERSION 3.21)
project(GRAPH)

set(CMAKE_CXX_STANDARD 20)

add_library(GRAPH STATIC Degrees.cpp View2.cpp View3.cpp View4.cpp algorithms.cpp View2-Linked.cpp LinkedDirectedGraph.cpp View2-LinkedWithWeight.cpp)

target_include_directories(GRAPH PUBLIC ${PROJECT_SOURCE_DIR}/include)
