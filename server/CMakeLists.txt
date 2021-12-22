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

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
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

set(
    TDS_DEV_SOURCES

    src/cli/user_commands/user_add_command.cpp
    src/cli/user_commands/user_passwd_command.cpp
    src/cli/user_commands/user_perms_command.cpp
    src/cli/user_commands/user_remove_command.cpp
    src/cli/user_commands/user_subcommand_base.cpp

    src/cli/help_command.cpp
    src/cli/init_command.cpp
    src/cli/invalid_command_arguments_error.cpp
    src/cli/no_such_command_error.cpp
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
    src/ip/tcp_socket.cpp

    src/linux/epoll_buffer.cpp
    src/linux/epoll_device.cpp
    src/linux/hash.cpp
    src/linux/io_device.cpp
    src/linux/linux_error.cpp
    src/linux/pipe_device.cpp
    src/linux/signal_device.cpp
    src/linux/terminal.cpp

    src/server/client_context.cpp
    src/server/client_pool.cpp
    src/server/client_service_supervisor.cpp
    src/server/client_service.cpp
    src/server/client.cpp
    src/server/server.cpp

    src/user/permissions.cpp
    src/user/user_table.cpp
)

option(TDS_STATIC "Build static library instead of dynamic.")
if(TDS_STATIC)
    add_library(tds-dev STATIC ${TDS_DEV_SOURCES})
else()
    add_library(tds-dev SHARED ${TDS_DEV_SOURCES})
endif()

target_include_directories(tds-dev PUBLIC include)
target_link_libraries(tds-dev PRIVATE fmt::fmt spdlog::spdlog crypt)
target_link_libraries(tds-dev PUBLIC tomlplusplus::tomlplusplus)

add_executable(tds src/main.cpp)
target_link_libraries(tds PRIVATE tomlplusplus::tomlplusplus)
target_link_libraries(tds PUBLIC tds-dev)

include("cmake/install.cmake")

option(TDS_TESTS "Enable tests for TinDox Server.")
option(TDS_UNIT_TESTS "Run unit tests for TinDox Server.")
option(TDS_INTEGRATION_TESTS "Run integration tests for TinDox Server.")

if(TDS_TESTS)
    enable_testing()
    target_compile_definitions(tds-dev PRIVATE TDS_NO_TERMIOS)

    if(TDS_UNIT_TESTS)
        include("cmake/unit_tests.cmake")
    endif()

    if(TDS_INTEGRATION_TESTS)
        include("cmake/integration_tests.cmake")
    endif()
endif()
