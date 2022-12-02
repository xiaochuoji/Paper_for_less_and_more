cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "-std=c++11")

project(Demo)


include_directories( "../"
                                          "/usr/local/include/eigen3"
                                          "/usr/local/include")

add_library(graph creatfigure.cc)
find_package(ortools  REQUIRED)

message(STATUS ${ortools})

add_executable(demo demo.cc)

target_link_libraries(demo graph ortools)  
