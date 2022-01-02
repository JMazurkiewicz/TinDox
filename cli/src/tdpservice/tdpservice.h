#ifndef TINDOX_CONSOLE_CLIENT_TDPSERVICE_H
#define TINDOX_CONSOLE_CLIENT_TDPSERVICE_H

#include "connection.h"

enum ConError {
    NONE,
    E_CON_TO_SERV,
    E_CLOSE_CON,
    E_CLOSED_CON,
};

class TDPService {
public:
    TDPService() = default;
    ~TDPService() {
        closeConnection();
    }

    bool initConnection(const string& serv_ip, int serv_port);
    bool closeConnection();

    std::string response = "";

private:
    Connection connectionToServer;
    ConError error_message = NONE;

};


#endif //TINDOX_CONSOLE_CLIENT_TDPSERVICE_H
