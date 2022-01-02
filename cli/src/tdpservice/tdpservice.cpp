#include "tdpservice.h"
#include <sstream>

bool TDPService::initConnection(const string& serv_ip, int serv_port) {
    try {
        connectionToServer.connectToServer(serv_ip, serv_port);
        std::string greetings = "";
        connectionToServer.receiveAllReadyFromServer(greetings);
        if(greetings.starts_with("Hello") && greetings.ends_with("\n")) {
            response = greetings;
            return true;
        }
    } catch (const std::system_error& ex) {
        error_message = E_CON_TO_SERV;
    }

    return false;
}

bool TDPService::closeConnection() {
    try {
        std::string exit_command = "exit\n\n";
        if(!connectionToServer.sendToServer(exit_command)) {
            error_message = E_CLOSED_CON;
            return false;
        } //czytanie odpowiedzi serwera ...
        connectionToServer.closeConnection();
    }  catch (const std::system_error& ex) {
        error_message = E_CLOSE_CON;
        return false;
    }

    return true;
}