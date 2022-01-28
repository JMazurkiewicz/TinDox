#ifndef TINDOX_CONSOLE_CLIENT_MODALWINDOW_H
#define TINDOX_CONSOLE_CLIENT_MODALWINDOW_H

#include <memory>
#include <string>
#include <utility>

class ModalWindow {

public:

    ModalWindow(std::string mess_first_line, std::string mess_second_line) : first_line(std::move(mess_first_line)),
                                                                             second_line(std::move(mess_second_line)) {}

    void showModalWindow();

private:
    std::string first_line;
    std::string second_line;
};


#endif //TINDOX_CONSOLE_CLIENT_MODALWINDOW_H
