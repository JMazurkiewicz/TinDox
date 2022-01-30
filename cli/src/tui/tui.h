#ifndef TINDOX_CONSOLE_CLIENT_TUI_H
#define TINDOX_CONSOLE_CLIENT_TUI_H

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include "return_exception.h"
#include "../tdpservice/tdpservice.h"

using std::string, std::vector;

namespace ftxui {
    class ScreenInteractive;
}

class Tui {
public:
    Tui(std::string ip, int port) : server_ip(std::move(ip)), server_port(port) { system("clear"); }

    ~Tui() { system("clear"); }

    void runTDPClient();

private:
    TDPService tdpService;
    std::string server_ip;
    int server_port;
    bool needToReconnect = true;
    bool terminateProgram = false;

    void showLoginView();

    void connectToServer();

    void logInUser(string &login, string &password);

    void showFilesView();

    bool updateFilesEntries(string &path, std::vector<string> &entries, vector<string> &f_names);

    bool getUserName(string &name);

    bool changeDirectory(string &current_location, const string &dir_name, vector<string> &entries,
                         vector<string> &names, int &selected_entry);

    bool checkIfShouldReconnect();

    bool deleteFileView(const string &file_to_delete);
};


#endif //TINDOX_CONSOLE_CLIENT_TUI_H
