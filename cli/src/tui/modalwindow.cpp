#include "modalwindow.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

void ModalWindow::showModalWindow() {
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    auto okButton = Button("   Ok   ", screen.ExitLoopClosure());

    auto modalWindow = Renderer(okButton, [&] {
        return vbox({
                            center(text(first_line)),
                            center(text(second_line)),
                            separator(),
                            okButton->Render() | center
                    }) | border | bgcolor(Color::Red) | clear_under;
    });

    screen.Loop(modalWindow);
}
