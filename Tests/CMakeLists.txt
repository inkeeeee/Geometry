cmake_minimum_required(VERSION 3.16)
project(Tests)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX)
    set(gtest_source_url https://github.com/google/googletest/archive/release-1.11.0.tar.gz)
elseif(WIN32)
    set(gtest_source_url https://github.com/google/googletest/archive/release-1.11.0.zip)
else()
    message(FATAL_ERROR "Операционная система не поддерживается")
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL ${gtest_source_url}
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(Tests 
    source/test_matrix.cpp
    source/test_polyline.cpp
    source/main.cpp
)

target_link_libraries(Tests 
    gtest
    gtest_main
    Geometry
)

target_include_directories(Tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/Geometry/include
)
