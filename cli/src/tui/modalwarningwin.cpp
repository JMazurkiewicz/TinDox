#include "modalwarningwin.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

void ModalWarningWin::showModalWindow() {
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    auto okButton = Button("   Ok   ", screen.ExitLoopClosure());

    auto modalWindow = Renderer(okButton, [&] {
        return vbox({
                            center(text(first_line)),
                            center(text(second_line)),
                            separator(),
                            okButton->Render() | center
                    }) | border | bgcolor(isNegative? Color::Red : Color::GrayDark) | clear_under;
    });

    screen.Loop(modalWindow);
}
