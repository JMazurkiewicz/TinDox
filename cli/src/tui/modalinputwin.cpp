#include "modalinputwin.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

bool ModalInputWin::showModalWindow(std::string &name) {
    using namespace ftxui;

    bool proceedWithOperation = false;

    auto inputName = Input(&name, "");
    auto screen = ScreenInteractive::TerminalOutput();
    auto cancelButton = Button("   Cancel   ", screen.ExitLoopClosure());
    auto operationButton = Button("   " + oper_button_txt + "   ", [&] {
        proceedWithOperation = true;
        cancelButton->OnEvent(Event::Return);
    });

    auto buttons = Container::Horizontal({operationButton, cancelButton});
    auto components = Container::Vertical({inputName, buttons});

    auto modalWindow = Renderer(components, [&] {
        return vbox({
                            center(text(instr_text)),
                            window(text(""), inputName->Render()) | size(WIDTH, GREATER_THAN, 15) | center,
                            separator(),
                            buttons->Render() | center
                    }) | border | bgcolor(Color::GrayDark) | clear_under;
    });

    screen.Loop(modalWindow);

    return proceedWithOperation;
}