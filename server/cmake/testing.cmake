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

    tests/unit/dummy.cpp
    tests/unit/dummy2.cpp
)

add_executable(tds-unit-tests ${TDS_UNIT_TESTS})
target_link_libraries(tds-unit-tests PRIVATE Catch2::Catch2WithMain tds-dev)
catch_discover_tests(tds-unit-tests)