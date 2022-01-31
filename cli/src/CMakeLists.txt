project(TinDox---console-client)

add_subdirectory(tdpservice/)
add_subdirectory(tui/)

# The main program
add_executable(cli
        main.cpp)

target_link_libraries(cli PRIVATE tdpservice tui)