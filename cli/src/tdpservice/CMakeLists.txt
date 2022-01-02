project(TinDox---console-client)

# All sources that also need to be tested in unit tests go into a static library
add_library(tdpservice STATIC
        tdpservice.h
        tdpservice.cpp
        connection.h
        connection.cpp
        )

target_include_directories(tdpservice PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})