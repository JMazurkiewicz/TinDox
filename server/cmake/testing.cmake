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
enable_testing()

set(
    TDS_UNIT_TESTS

    tests/unit/cli/cli_error_test.cpp
    tests/unit/cli/command_interface_test.cpp
    tests/unit/cli/command_runner_test.cpp
    tests/unit/cli/command_test.cpp
    tests/unit/cli/config_command_test.cpp
    tests/unit/cli/help_command_test.cpp
    tests/unit/cli/init_command_test.cpp
    tests/unit/cli/log_command_test.cpp
    tests/unit/cli/run_command_test.cpp
    tests/unit/cli/user_command_test.cpp

    tests/unit/ip/address_v4_test.cpp
    tests/unit/ip/ip_error_test.cpp
)

add_executable(tds-unit-tests ${TDS_UNIT_TESTS})
target_link_libraries(tds-unit-tests PRIVATE Catch2::Catch2WithMain tds-dev)
catch_discover_tests(tds-unit-tests)
