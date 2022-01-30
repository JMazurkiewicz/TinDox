project(TinDox---console-client)

add_library(tui STATIC
        tui.cpp
        tui.h
        modalwarningwindow.cpp
        modalwarningwindow.h
        return_exception.h modaldecisionwin.cpp modaldecisionwin.h)

target_include_directories(tui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tui PRIVATE ftxui::screen ftxui::dom ftxui::component)