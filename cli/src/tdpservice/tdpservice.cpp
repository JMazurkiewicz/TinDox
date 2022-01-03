#include "tdpservice.h"
#include <sstream>

bool TDPService::initConnection(const string& serv_ip, int serv_port) {
    try {
        connectionToServer->connectToServer(serv_ip, serv_port);
    } catch (const std::system_error& ex) {
        error_code = E_CON_TO_SERV;
        return false;
    }

    return true;
}

bool TDPService::closeConnection() {

    if (sendCommand("exit", "", "", "", "", "", ""))
    {
        try {
            connectionToServer->closeConnection();
            return true;
        }  catch (const std::system_error& ex) {
            error_code = E_CLOSE_CON;
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

bool TDPService::auth(string login, string passwd) {
    if (sendCommand("auth", "login", login, "passwd", passwd, "", "")
        && (error_code = responseAnalyzer.readSingleLineResponse("auth")) == OK) {
        response_first_line = std::to_string(error_code) + " auth\n\n";
        return true;
    } else
        return false;
}

bool TDPService::mkdir(string name) {
    if (sendCommand("mkdir", "name", name, "", "", "", "")
        && (error_code = responseAnalyzer.readSingleLineResponse("mkdir")) == OK) {
        response_first_line = std::to_string(error_code) + " mkdir\n\n";
        return true;
    } else
        return false;
}

bool TDPService::rm(string name) {
    if (sendCommand("rm", "name", name, "", "", "", "")
        && (error_code = responseAnalyzer.readSingleLineResponse("rm")) == OK) {
        response_first_line = std::to_string(error_code) + " rm\n\n";
        return true;
    } else
        return false;
}

bool TDPService::rename(string oname, string nname) {
    if (sendCommand("rename", "oname", oname, "nname", nname, "", "")
        && (error_code = responseAnalyzer.readSingleLineResponse("rename")) == OK) {
        response_first_line = std::to_string(error_code) + " rename\n\n";
        return true;
    } else
        return false;
}
bool TDPService::cp(string name, string path) {
    if (sendCommand("cp", "name", name, "path", path, "", "")
        && (error_code = responseAnalyzer.readSingleLineResponse("cp")) == OK) {
        response_first_line = std::to_string(error_code) + " cp\n\n";
        return true;
    } else
        return false;
}

bool TDPService::mv(string name, string path) {
    if (sendCommand("mv", "name", name, "path", path, "", "")
        && (error_code = responseAnalyzer.readSingleLineResponse("mv")) == OK) {
        response_first_line = std::to_string(error_code) + " mv\n\n";
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
        if(connectionToServer->sendToServer(full_command))
            return true;
        else
            error_code = E_CLOSED_CON;
    }  catch (const std::system_error& ex) {
        error_code = E_SEND_MESSAGE;
    }

    return false;
}
