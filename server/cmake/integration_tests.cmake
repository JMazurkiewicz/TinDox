find_program(BASH_PROGRAM bash)
if(NOT BASH_PROGRAM)
    message(FATAL_ERROR "CTest failed to run integration tests.")
endif()

set(TDS_INTEGRATION_TESTS_SOURCES help init run user)

foreach(TEST_NAME ${TDS_INTEGRATION_TESTS_SOURCES})
    add_test(
        NAME "INTEGRATION::${TEST_NAME}"
        COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/${TEST_NAME}.bash
    )
endforeach()
