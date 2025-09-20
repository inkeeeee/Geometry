cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

project(Lab1)

add_subdirectory(Geometry)
add_subdirectory(Graphics)
add_subdirectory(Tests)
add_subdirectory(examples)
add_subdirectory(CommandInterface)

add_custom_target(docs
    COMMAND doxygen Doxyfile.in
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating documentation with Doxygen"
)

add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}"
    COMMENT "Completely removing build directory"
)
