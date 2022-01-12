find_program(SH_PROGRAM sh)
if(NOT SH_PROGRAM)
    message(FATAL_ERROR "CTest failed to run integration tests.")
endif()

set(TDS_INTEGRATION_TESTS_CLI_TESTS help init run user version)
foreach(TEST_NAME ${TDS_INTEGRATION_TESTS_CLI_TESTS})
    add_test(
        NAME "INTEGRATION::cli_tests::${TEST_NAME}"
        COMMAND ${SH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/cli_tests/${TEST_NAME}.sh
    )
endforeach()

set(TDS_INTEGRATION_TESTS_REQUEST_TESTS auth cd rm)
foreach(TEST_NAME ${TDS_INTEGRATION_TESTS_REQUEST_TESTS})
    add_test(
        NAME "INTEGRATION::request_tests::${TEST_NAME}"
        COMMAND ${SH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/request_tests/${TEST_NAME}.sh
    )
endforeach()
