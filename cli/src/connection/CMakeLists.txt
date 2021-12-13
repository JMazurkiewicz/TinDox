project(TinDox---console-client)

# All sources that also need to be tested in unit tests go into a static library
add_library(connection STATIC
            connection.h
            connection.cpp
            )

target_include_directories(connection PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})