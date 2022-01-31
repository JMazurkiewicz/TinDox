#ifndef TINDOX_CONSOLE_CLIENT_MODALINPUTWIN_H
#define TINDOX_CONSOLE_CLIENT_MODALINPUTWIN_H

#include <memory>
#include <string>
#include <utility>

class ModalInputWin {
public:

    ModalInputWin(std::string instruction, std::string button_txt) : instr_text(
            std::move(instruction)), oper_button_txt(std::move(button_txt)) {}

    bool showModalWindow(std::string &name);

private:
    std::string instr_text;
    std::string oper_button_txt;
};


#endif //TINDOX_CONSOLE_CLIENT_MODALINPUTWIN_H
