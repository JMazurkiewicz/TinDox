find_package(Git REQUIRED)

execute_process(
    COMMAND ${GIT_EXECUTABLE} submodule update --init --progress
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    RESULT_VARIABLE GIT_SUBMODULE_RESULT
)

if(NOT GIT_SUBMODULE_RESULT EQUAL "0")
    message(FATAL_ERROR "Updating submodule 'Catch2' failed with: '${GIT_SUBMODULE_RESULT}'.")
endif()

set(CATCH2_PATH ${PROJECT_SOURCE_DIR}/extern/Catch2)
add_subdirectory(${CATCH2_PATH} "Catch2" EXCLUDE_FROM_ALL)
list(APPEND CMAKE_MODULE_PATH ${CATCH2_PATH}/extras)
