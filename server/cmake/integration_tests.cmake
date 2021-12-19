find_program(BASH_PROGRAM bash)
if(NOT BASH_PROGRAM)
    message(FATAL_ERROR "CTest failed to run integration tests.")
endif()

function(AddIntegrationTest TEST_NAME)
    add_test(NAME ${TEST_NAME} COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration/${TEST_NAME}.bash)
endfunction()

enable_testing()
AddIntegrationTest(help)
AddIntegrationTest(init)
AddIntegrationTest(run)
AddIntegrationTest(user)
