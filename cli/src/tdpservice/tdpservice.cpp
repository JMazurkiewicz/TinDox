#include "tdpservice.h"
#include <sstream>

bool TDPService::initConnection(const string& serv_ip, int serv_port) {
    try {
        connectionToServer.connectToServer(serv_ip, serv_port);
        string greetings;
        connectionToServer.receiveAllReadyFromServer(greetings);
        if(greetings.starts_with("Hello") && greetings.ends_with("\n")) {
            response_first_line = greetings;
            return true;
        }
    } catch (const std::system_error& ex) {
        error_code = E_CON_TO_SERV;
    }

    return false;
}

bool TDPService::closeConnection() {

    if (sendCommand("exit", "", "", "", "", "", ""))
    {
        if((error_code = responseAnalyzer.readSingleLineResponse("exit")) == OK) {
            response_first_line = std::to_string(error_code) + " exit\n\n";
            try {
                connectionToServer.closeConnection();
                return true;
            }  catch (const std::system_error& ex) {
                error_code = E_CLOSE_CON;
            }
        }
    }
    return false;
}

bool TDPService::logout() {
    if (sendCommand("logout", "", "", "", "", "", "")
        && (error_code = responseAnalyzer.readSingleLineResponse("logout")) == OK) {
        response_first_line = std::to_string(error_code) + " logout\n\n";
        return true;
    } else
        return false;
}

bool TDPService::sendCommand(string command_name, string field_name1, string field_value1, string field_name2,
                             string field_value2, string field_name3, string field_value3) {

    if(command_name.size() > 6 || field_name1.size() + field_value1.size() >= BUF_MAX_SIZE-1
    || field_name2.size() + field_value2.size() >= BUF_MAX_SIZE-1
    || field_name3.size() + field_value3.size() >= BUF_MAX_SIZE-1) {
        error_code = E_LINE_TOO_LONG;
        return false;
    }

    string full_command = command_name + "\n";
    if(!field_name1.empty()) {
        full_command += field_name1 + ":" + field_value1 + "\n";
        if (!field_name2.empty()) {
            full_command += field_name2 + ":" + field_value2 + "\n";
            if (!field_name3.empty())
                full_command += field_name3 + ":" + field_value3 + "\n";
        }
    }
    full_command += "\n";

    try {
        if(connectionToServer.sendToServer(full_command))
            return true;
        else
            error_code = E_CLOSED_CON;
    }  catch (const std::system_error& ex) {
        error_code = E_SEND_MESSAGE;
    }

    return false;
}
