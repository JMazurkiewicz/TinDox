#ifndef TINDOX_CONSOLE_CLIENT_MODALWARNINGWIN_H
#define TINDOX_CONSOLE_CLIENT_MODALWARNINGWIN_H

#include <memory>
#include <string>
#include <utility>

class ModalWarningWin {

public:

    ModalWarningWin(std::string mess_first_line, std::string mess_second_line, bool negative = true) : first_line(
            std::move(mess_first_line)), second_line(std::move(mess_second_line)), isNegative(negative) {}

    void showModalWindow();

private:
    std::string first_line;
    std::string second_line;
    bool isNegative;
};


#endif //TINDOX_CONSOLE_CLIENT_MODALWARNINGWIN_H