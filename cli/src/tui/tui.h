#ifndef TINDOX_CONSOLE_CLIENT_TUI_H
#define TINDOX_CONSOLE_CLIENT_TUI_H

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "return_exception.h"
#include "../tdpservice/tdpservice.h"

using std::string;

namespace ftxui {
    class ScreenInteractive;
}

class Tui {
public:
    Tui(std::string ip, int port) :server_ip(std::move(ip)), server_port(port) { system("clear");}
    ~Tui() { system("clear");}

    void modal();
    void menu_framed();
    void select_menu();
    void menu_style();
    void nested_menus();
    void dropdown();
    void test();

    void runTDPClient();

private:
    TDPService tdpService;
    std::string server_ip;
    int server_port;
    bool needToReconnect = true;
    bool terminateProgram = false;

    static void Nested(string path);

    void showLoginView();

    void connectToServer();

    void logInUser(string &login, string &password);

    void showFilesView();
};


#endif //TINDOX_CONSOLE_CLIENT_TUI_H
