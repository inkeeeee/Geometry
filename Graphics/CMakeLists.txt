cmake_minimum_required(VERSION 3.16)

add_library(Graphics INTERFACE)
target_include_directories(Graphics
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/Geometry/include
)

target_link_libraries(Graphics
        INTERFACE
        Geometry
)
