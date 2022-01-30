#ifndef TINDOX_CONSOLE_CLIENT_MODALINPUTWIN_H
#define TINDOX_CONSOLE_CLIENT_MODALINPUTWIN_H

#include <memory>
#include <string>
#include <utility>

class ModalInputWin {
public:

    ModalInputWin(std::string instruction, std::string button_txt, bool winOnEnter) : instr_text(
            std::move(instruction)), oper_button_txt(std::move(button_txt)), pickPathOnEnter(winOnEnter) {}

    void showModalWindow();

private:
    std::string instr_text;
    std::string oper_button_txt;
    bool pickPathOnEnter;
};


#endif //TINDOX_CONSOLE_CLIENT_MODALINPUTWIN_H
