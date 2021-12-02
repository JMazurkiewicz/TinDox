cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project("TinDox_Server" LANGUAGES CXX VERSION 0.0.1)

if(NOT UNIX OR APPLE)
    message(FATAL_ERROR "This project is only supported by Linux OS.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(
    TDS_DEV_SOURCES

    src/hello.cpp
)

add_library(tds-dev ${TDS_DEV_SOURCES})
target_include_directories(tds-dev PUBLIC include)

set(
    TDS_SOURCES

    src/main.cpp
)

add_executable(tds ${TDS_SOURCES})
target_link_libraries(tds PUBLIC tds-dev)

if(TDS_TESTING)
    include("cmake/testing.cmake")
endif()
