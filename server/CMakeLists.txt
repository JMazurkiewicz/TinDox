cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(
    TinDoxServer
    LANGUAGES CXX
    VERSION 0.0.1
    HOMEPAGE_URL https://github.com/JMazurkiewicz/TinDox/server
    DESCRIPTION "TinDox Server"
)

if(NOT UNIX OR APPLE)
    message(FATAL_ERROR "This project is only supported by Linux OS.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

find_package(tomlplusplus 2.6 QUIET)
if(tomlplusplus_FOUND)
    message(STATUS "Found tomlplusplus: ${tomlplusplus_DIR} (found version \"${tomlplusplus_VERSION}\")")
else()
    FetchContent_Declare(
        tomlplusplus
        GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
        GIT_TAG v2.5.0
    )

    if(NOT tomlplusplus_POPULATED)
        FetchContent_Populate(tomlplusplus)
        add_subdirectory("${tomlplusplus_SOURCE_DIR}" EXCLUDE_FROM_ALL)
    endif()
endif()

find_package(fmt 8.0 QUIET)
if(fmt_FOUND)
    message(STATUS "Found {fmt}: ${fmt_DIR} (found version \"${fmt_VERSION}\")")
else()
    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 8.0.1
    )

    if(NOT fmt_POPULATED)
        FetchContent_Populate(fmt)
        add_subdirectory("${fmt_SOURCE_DIR}" EXCLUDE_FROM_ALL)
    endif()
endif()

set(PIC_FLAG CMAKE_POSITION_INDEPENDENT_CODE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package(spdlog 1.9 QUIET)
if(spdlog_FOUND)
    message(STATUS "Found spdlog: ${spdlog_DIR} (found version \"${spdlog_VERSION}\")")
else()
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.9.2
    )

    if(NOT spdlog_POPULATED)
        FetchContent_Populate(spdlog)
        add_subdirectory("${spdlog_SOURCE_DIR}" EXCLUDE_FROM_ALL)
    endif()
endif()
set(CMAKE_POSITION_INDEPENDENT_CODE PIC_FLAG)

set(
    TDS_DEV_SOURCES

    src/cli/command_base.cpp
    src/cli/config_command.cpp
    src/cli/help_command.cpp
    src/cli/init_command.cpp
    src/cli/log_command.cpp
    src/cli/run_command.cpp
    src/cli/user_command.cpp

    src/config/config_reader.cpp
    src/config/defaults.cpp
    src/config/server_config.cpp

    src/ip/address_v4.cpp
    src/ip/endpoint_v4.cpp
    src/ip/ip_error.cpp
    src/ip/port.cpp
    src/ip/tcp_listener.cpp

    src/linux/epoll_buffer.cpp
    src/linux/epoll_device.cpp
    src/linux/io_device.cpp
    src/linux/linux_error.cpp
    src/linux/signal_device.cpp

    src/server/server.cpp
)

add_library(tds-dev SHARED ${TDS_DEV_SOURCES})
target_include_directories(tds-dev PUBLIC include)
target_link_libraries(tds-dev PRIVATE tomlplusplus::tomlplusplus fmt::fmt spdlog::spdlog)

set(
    TDS_SOURCES

    src/main.cpp
)

add_executable(tds ${TDS_SOURCES})
target_link_libraries(tds PUBLIC tds-dev)

include("cmake/installing.cmake")

option(TDS_TESTING "Run unit tests.")
if(TDS_TESTING)
    include("cmake/testing.cmake")
endif()
