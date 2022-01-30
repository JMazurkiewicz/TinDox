#ifndef TINDOX_CONSOLE_CLIENT_MODALDECISIONWIN_H
#define TINDOX_CONSOLE_CLIENT_MODALDECISIONWIN_H

#include <memory>
#include <string>
#include <utility>

class ModalDecisionWin {
public:

    ModalDecisionWin(std::string mess_first_line, std::string mess_second_line) : question(std::move(mess_first_line)),
                                                                                  second_line(std::move(mess_second_line)) {}

    void showModalWindow();

private:
    std::string question;
    std::string second_line;
};


#endif //TINDOX_CONSOLE_CLIENT_MODALDECISIONWIN_H
