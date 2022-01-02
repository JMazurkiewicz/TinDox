project(TinDox---console-client)

add_subdirectory(tdpservice/)

# The main program
add_executable(cli
        main.cpp)

target_link_libraries(cli PRIVATE tdpservice)