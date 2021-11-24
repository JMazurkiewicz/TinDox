cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project("TinDox Server" LANGUAGES CXX VERSION 0.0.1)

if(NOT UNIX OR APPLE)
    message(FATAL_ERROR "This project is only support by Linux OS.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

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
target_link_libraries(tds tds-dev)

if(TDS_TESTING)
    find_package(Catch2 REQUIRED)
    include(CTest)
    include(Catch)
    enable_testing()

    set(
        TDS_UNIT_TESTS

        tests/unit/dummy.cpp
        tests/unit/dummy2.cpp
    )
    add_executable(tds-unit-tests ${TDS_UNIT_TESTS})
    target_link_libraries(tds-unit-tests PRIVATE Catch2::Catch2WithMain tds-dev)
    catch_discover_tests(tds-unit-tests)
endif()
