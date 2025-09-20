cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

add_library(Geometry INTERFACE)

target_include_directories(Geometry
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
