#ifndef TINDOX_CONSOLE_CLIENT_TDPSERVICE_H
#define TINDOX_CONSOLE_CLIENT_TDPSERVICE_H

#include "connection.h"
#include "responseanalyzer.h"

class TDPService {
public:
    TDPService() : connectionToServer(std::make_shared<Connection>()), responseAnalyzer(connectionToServer) {}

    ~TDPService() {
        closeConnection();
    }

    bool initConnection(const string& serv_ip, int serv_port);
    bool closeConnection();
    bool logout();
    bool auth(string login, string passwd);
    bool mkdir(string name);
    bool rm(string name);
    bool rename(string oname, string nname);
    bool cp(string name, string path);
    bool mv(string name, string path);

    string response_first_line;
    ConnectionError error_code = NONE;

private:
    shared_ptr<Connection> connectionToServer;
    ResponseAnalyzer responseAnalyzer;

    bool sendCommand(string command_name, string field_name1, string field_value1, string field_name2, string field_value2,
                string field_name3, string field_value3);
};


#endif //TINDOX_CONSOLE_CLIENT_TDPSERVICE_H
