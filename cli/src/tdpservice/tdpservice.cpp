#include "tdpservice.h"
#include <sstream>

bool TDPService::initConnection(const string &serv_ip, int serv_port) {
    try {
        connectionToServer.connectToServer(serv_ip, serv_port);
    } catch (const std::system_error &ex) {
        error_code = E_CON_TO_SERV;
        return false;
    }

    return true;
}

bool TDPService::closeConnection() {
    if (sendAndGetResponse("exit", "", "", "", "", "", "")) {
        try {
            connectionToServer.closeConnection();
            return true;
        } catch (const std::runtime_error &ex) {
            error_code = E_CLOSE_CON;
        }
    }
    return false;
}

bool TDPService::logout() {
    if (sendAndGetResponse("logout", "", "", "", "", "", "")
        &&
        (error_code = responseAnalyzer.analyseSimpleResponse("logout", received_response, response_body, false)) == OK)
        return true;
    else
        return false;
}

bool TDPService::auth(const string &login, const string &passwd) {
    if (sendAndGetResponse("auth", "login", login, "passwd", passwd, "", "")
        && (error_code = responseAnalyzer.analyseSimpleResponse("auth", received_response, response_body, false)) == OK)
        return true;
    else
        return false;
}

bool TDPService::mkdir(const string &name) {
    if (sendAndGetResponse("mkdir", "name", name, "", "", "", "")
        &&
        (error_code = responseAnalyzer.analyseSimpleResponse("mkdir", received_response, response_body, false)) == OK)
        return true;
    else
        return false;
}

bool TDPService::rm(const string &name) {
    if (sendAndGetResponse("rm", "name", name, "", "", "", "")
        && (error_code = responseAnalyzer.analyseSimpleResponse("rm", received_response, response_body, true)) == OK)
        return true;
    else
        return false;
}

bool TDPService::rename(const string &oname, const string &nname) {
    if (sendAndGetResponse("rename", "oname", oname, "nname", nname, "", "")
        &&
        (error_code = responseAnalyzer.analyseSimpleResponse("rename", received_response, response_body, false)) == OK)
        return true;
    else
        return false;
}

bool TDPService::cp(const string &name, const string &path) {
    if (sendAndGetResponse("cp", "name", name, "path", path, "", "")
        && (error_code = responseAnalyzer.analyseSimpleResponse("cp", received_response, response_body, false)) == OK)
        return true;
    else
        return false;
}

bool TDPService::mv(const string &name, const string &path) {
    if (sendAndGetResponse("mv", "name", name, "path", path, "", "")
        && (error_code = responseAnalyzer.analyseSimpleResponse("mv", received_response, response_body, false)) == OK)
        return true;
    else
        return false;
}

bool TDPService::cd(const string &path) {
    if (sendAndGetResponse("cd", "path", path, "", "", "", "")
        && (error_code = responseAnalyzer.analyseSimpleResponse("cd", received_response, response_body, true)) == OK)
        return true;
    else
        return false;
}

bool TDPService::ls(const string &path, const string &size, const string &mod) {
    if (sendAndGetResponse("ls", path.empty() ? "" : "path", path,
                           size.empty() ? "" : "size", size, mod.empty() ? "" : "mod", mod)
        && (error_code = responseAnalyzer.analyseMultilineResponse("ls", received_response, response_body)) == OK)
        return true;
    else
        return false;
}

bool TDPService::tree(const string &path) {
    if (sendAndGetResponse("tree", path.empty() ? "" : "path", path,
                           "", "", "", "")
        && (error_code = responseAnalyzer.analyseMultilineResponse("tree", received_response, response_body)) == OK)
        return true;
    else
        return false;
}


bool TDPService::name() {
    if (sendAndGetResponse("name", "", "", "", "", "", "")
        && (error_code = responseAnalyzer.analyseSimpleResponse("name", received_response, response_body, true)) == OK)
        return true;
    else
        return false;
}

bool TDPService::perms() {
    if (sendAndGetResponse("perms", "", "", "", "", "", "")
        && (error_code = responseAnalyzer.analyseSimpleResponse("perms", received_response, response_body, true)) == OK)
        return true;
    else
        return false;
}

bool TDPService::pwd() {
    if (sendAndGetResponse("pwd", "", "", "", "", "", "")
        && (error_code = responseAnalyzer.analyseSimpleResponse("pwd", received_response, response_body, true)) == OK)
        return true;
    else
        return false;
}

bool TDPService::sendAndGetResponse(const string &command_name, const string &field_name1, const string &field_value1,
                                    const string &field_name2, const string &field_value2, const string &field_name3,
                                    const string &field_value3) {
    error_code = NONE;
    if (command_name.size() > 6 || field_name1.size() + field_value1.size() >= BUF_MAX_SIZE - 1
        || field_name2.size() + field_value2.size() >= BUF_MAX_SIZE - 1
        || field_name3.size() + field_value3.size() >= BUF_MAX_SIZE - 1) {
        error_code = E_LINE_TOO_LONG;
        return false;
    }

    last_sent_command.clear();
    last_sent_command = command_name + "\n";
    if (!field_name1.empty())
        last_sent_command += field_name1 + ":" + field_value1 + "\n";

    if (!field_name2.empty())
        last_sent_command += field_name2 + ":" + field_value2 + "\n";

    if (!field_name3.empty())

        last_sent_command += field_name3 + ":" + field_value3 + "\n";

    last_sent_command += "\n";
    received_response = last_sent_command;

    try {
        if (connectionToServer.exchangeWithServer(received_response))
            return true;
        else
            error_code = E_CLOSED_CON;
    } catch (const std::runtime_error &ex) {
        error_code = E_EXCHANGE_MESSAGE;
    }

    return false;
}
