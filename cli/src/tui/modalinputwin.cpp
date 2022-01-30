#include "modalinputwin.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

void ModalInputWin::showModalWindow() {
    using namespace ftxui;

    auto screen = ScreenInteractive::TerminalOutput();

    auto okButton = Button("   Ok   ", screen.ExitLoopClosure());

    auto modalWindow = Renderer(okButton, [&] {
        return vbox({
                            center(text(instr_text)),
                            center(text(oper_button_txt)),
                            separator(),
                            okButton->Render() | center
                    }) | border | bgcolor(Color::Red) | clear_under;
    });

    screen.Loop(modalWindow);
}