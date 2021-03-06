#ifndef TINDOX_CONSOLE_CLIENT_TUI_H
#define TINDOX_CONSOLE_CLIENT_TUI_H

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <filesystem>
#include "../tdpservice/tdpservice.h"

using std::string, std::vector;

class Tui {
public:
    Tui(std::string ip, int port) : server_ip(std::move(ip)), server_port(port) { system("clear"); }

    ~Tui() { system("clear"); }

    void runTDPClient();

private:
    const string PATH_TO_FOLDER = std::filesystem::current_path().string() + "/TinDox";
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

    bool createDir();

    bool renameFile(const string &old_name);

    bool showPathSelectWind(const string &instr_text, const string &oper_button_text, string &path);

    bool copyFile(const string &file_name, const string &current_location);

    bool moveFile(const string &file_name, const string &current_location);

    bool downloadFile(const string &file_name);

    bool uploadFile();
};


#endif //TINDOX_CONSOLE_CLIENT_TUI_H
