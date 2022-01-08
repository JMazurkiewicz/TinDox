find_package(Catch2 3 QUIET)
if(Catch2_FOUND)
    message(STATUS "Found Catch2: ${Catch2_DIR} (found version \"${Catch2_VERSION}\")")
else()
    include(FetchContent)
    set(FETCHCONTENT_QUIET OFF)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG devel
    )
    FetchContent_MakeAvailable(Catch2)

    list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")
endif()

include(CTest)
include(Catch)

set(
    TDS_UNIT_TESTS_SOURCES

    tests/unit/cli/cli_error_test.cpp
    tests/unit/cli/help_command_test.cpp
    tests/unit/cli/init_command_test.cpp
    tests/unit/cli/invalid_command_arguments_error_test.cpp
    tests/unit/cli/run_command_test.cpp
    tests/unit/cli/user_command_test.cpp

    tests/unit/command/command_executor_test.cpp
    tests/unit/command/command_test.cpp

    tests/unit/config/defaults_test.cpp
    tests/unit/config/server_config_test.cpp

    tests/unit/ip/address_v4_test.cpp
    tests/unit/ip/endpoint_v4_test.cpp
    tests/unit/ip/ip_error_test.cpp
    tests/unit/ip/port_test.cpp
    tests/unit/ip/tcp_listener_test.cpp

    tests/unit/linux/epoll_buffer_test.cpp
    tests/unit/linux/epoll_device_test.cpp
    tests/unit/linux/io_device_test.cpp
    tests/unit/linux/linux_error_test.cpp
    tests/unit/linux/pipe_device_test.cpp
    tests/unit/linux/signal_device_test.cpp

    tests/unit/protocol/field_test.cpp
    tests/unit/protocol/path_lock_test.cpp
    tests/unit/protocol/protocol_code_test.cpp
    tests/unit/protocol/protocol_interpreter_test.cpp
    tests/unit/protocol/receiver_test.cpp
    tests/unit/protocol/request_test.cpp
    tests/unit/protocol/response_test.cpp
    tests/unit/protocol/sender_test.cpp
    tests/unit/protocol/server_context_test.cpp

    tests/unit/user/permissions_test.cpp
)

add_executable(tds-unit-tests ${TDS_UNIT_TESTS_SOURCES})
target_link_libraries(tds-unit-tests PRIVATE Catch2::Catch2WithMain tds-dev Threads::Threads)
target_include_directories(tds-unit-tests PRIVATE tests/unit)
catch_discover_tests(tds-unit-tests TEST_PREFIX "UNIT::")
