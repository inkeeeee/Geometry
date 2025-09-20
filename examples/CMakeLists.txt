cmake_minimum_required(VERSION 3.16)
project(examples)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

add_executable (examples   source/main.cpp)

target_link_libraries(examples Geometry
                               Graphics )
