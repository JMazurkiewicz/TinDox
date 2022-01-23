cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

if(NOT UNIX OR APPLE)
    message(FATAL_ERROR "This project is only supported by Linux OS.")
endif()

project(
    TinDoxServer
    LANGUAGES CXX
    VERSION 0.1.2
    HOMEPAGE_URL https://github.com/JMazurkiewicz/TinDox/tree/main/server
    DESCRIPTION "TinDox Server"
)

include(cmake/checks.cmake)
include(FetchContent)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

find_package(fmt 8.1 QUIET)
if(fmt_FOUND)
    message(STATUS "Found {fmt}: ${fmt_DIR} (found version \"${fmt_VERSION}\")")
else()
    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 8.1.1
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
    src/cli/run_command.cpp
    src/cli/user_command.cpp
    src/cli/version_command.cpp

    src/config/config_reader.cpp
    src/config/defaults.cpp
    src/config/server_config.cpp

    src/ip/address_v4.cpp
    src/ip/endpoint_v4.cpp
    src/ip/port.cpp
    src/ip/tcp_listener.cpp
    src/ip/tcp_socket.cpp

    src/linux/echoless_terminal_guard.cpp
    src/linux/epoll_buffer.cpp
    src/linux/epoll_device.cpp
    src/linux/file_device.cpp
    src/linux/hash.cpp
    src/linux/io_device.cpp
    src/linux/linux_error.cpp
    src/linux/pipe_device.cpp
    src/linux/signal_device.cpp
    src/linux/terminal.cpp
    src/linux/transfer_bytes.cpp

    src/protocol/execution/auth.cpp
    src/protocol/execution/cd.cpp
    src/protocol/execution/command_base.cpp
    src/protocol/execution/cp.cpp
    src/protocol/execution/dl.cpp
    src/protocol/execution/dls.cpp
    src/protocol/execution/exit.cpp
    src/protocol/execution/logout.cpp
    src/protocol/execution/ls.cpp
    src/protocol/execution/mkdir.cpp
    src/protocol/execution/mv.cpp
    src/protocol/execution/name.cpp
    src/protocol/execution/path_based_command.cpp
    src/protocol/execution/perms.cpp
    src/protocol/execution/pwd.cpp
    src/protocol/execution/rename.cpp
    src/protocol/execution/rm.cpp
    src/protocol/execution/transport_command.cpp
    src/protocol/execution/tree.cpp
    src/protocol/execution/ul.cpp

    src/protocol/auth_token.cpp
    src/protocol/client_context.cpp
    src/protocol/download_manager.cpp
    src/protocol/download_token.cpp
    src/protocol/field.cpp
    src/protocol/path_lock.cpp
    src/protocol/protocol_error.cpp
    src/protocol/protocol_interpreter.cpp
    src/protocol/receiver.cpp
    src/protocol/request.cpp
    src/protocol/response_builder.cpp
    src/protocol/response.cpp
    src/protocol/sender.cpp
    src/protocol/server_context.cpp
    src/protocol/upload_manager.cpp
    src/protocol/upload_token.cpp

    src/server/client_pool.cpp
    src/server/client_service_supervisor.cpp
    src/server/client_service.cpp
    src/server/client.cpp
    src/server/server_logger.cpp
    src/server/server.cpp

    src/user/permissions.cpp
    src/user/user_table.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

configure_file(cmake/config.hpp.in include/tds/config.hpp)

add_library(tds-dev SHARED ${TDS_DEV_SOURCES})
target_compile_features(tds-dev PUBLIC cxx_std_20)
target_include_directories(tds-dev PRIVATE ${PROJECT_BINARY_DIR}/include)
target_include_directories(tds-dev PUBLIC include)
target_link_libraries(tds-dev PRIVATE crypt Threads::Threads fmt::fmt spdlog::spdlog)
target_link_libraries(tds-dev PUBLIC tomlplusplus::tomlplusplus)

add_executable(tds src/main.cpp)
target_link_libraries(tds PUBLIC tds-dev)

include("cmake/install.cmake")

option(TDS_TESTS "Enable tests for TinDox Server.")
if(TDS_TESTS)
    enable_testing()
    include("cmake/unit_tests.cmake")
    include("cmake/integration_tests.cmake")
endif()
