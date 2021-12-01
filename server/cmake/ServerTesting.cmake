find_package(Catch2 3 REQUIRED)
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
