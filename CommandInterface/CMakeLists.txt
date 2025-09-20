cmake_minimum_required(VERSION 3.16)
project(CommandInterface)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CommandInterface 
    source/CommandInterface.cpp
    source/main.cpp
)
target_link_libraries(CommandInterface
    Geometry
    Graphics
)

target_include_directories(CommandInterface
    PUBLIC
    ${CMAKE_SOURCE_DIR}/Geometry/include
    ${CMAKE_SOURCE_DIR}/CommandInterface/include
    ${CMAKE_SOURCE_DIR}/Graphics/include
)
