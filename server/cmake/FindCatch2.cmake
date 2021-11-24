set(CATCH2_PATH ${PROJECT_SOURCE_DIR}/extern/Catch2)
add_subdirectory(${CATCH2_PATH})
list(APPEND CMAKE_MODULE_PATH ${CATCH2_PATH}/extras)
