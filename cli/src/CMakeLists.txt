project(TinDox---console-client)

add_subdirectory(connection/)

# The main program
add_executable(ccl
        main.cpp)

target_link_libraries(ccl PRIVATE connection)