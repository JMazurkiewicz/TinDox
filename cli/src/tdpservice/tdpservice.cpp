#include "tdpservice.h"
#include <sstream>
#include <fcntl.h>
#include <sys/stat.h>

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
    if (sendAndGetResponse("auth", "login", login.empty() ? "." : login, "passwd", passwd.empty() ? "." : passwd, "",
                           "")
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

bool TDPService::ul(const string &name, const string &path, const bool &retry) {
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1)
        return false;

    struct stat st = {};
    if (fstat(fd, &st) == -1) {
        close(fd);
        return false;
    }

    size_t size = st.st_size;

    try {
        if (sendAndGetResponse("ul", "name", name, "size", std::to_string(size), "retry", retry ? "true" : "false")
            &&
            (error_code = responseAnalyzer.analyseSimpleResponse("ul", received_response, response_body, true)) == OK) {

            off_t offset = std::stol(response_body);
            bool result = connectionToServer.uploadFile(fd, offset, size);
            close(fd);
            return result;
        }
    } catch (std::runtime_error &ex) {
        close(fd);
    }
    return false;
}

bool TDPService::dl(const string &name, const string &path, const bool &retry) {
    int fd;
    if (retry)
        fd = open(path.c_str(), O_RDWR | O_APPEND);
    else
        fd = open(path.c_str(), O_RDWR | O_TRUNC | O_CREAT);

    if (fd == -1)
        return false;

    struct stat st = {};
    if (fstat(fd, &st) == -1) {
        close(fd);
        return false;
    }
    size_t size = st.st_size;

    try {
        if (sendAndGetResponse("dl", "name", name, retry ? "size" : "", retry ? std::to_string(size) : "", "", "")
            &&
            (error_code = responseAnalyzer.analyseSimpleResponse("dl", received_response, response_body, true)) == OK) {

            size_t bytes_to_read = std::stoul(response_body);
            bool result = downloadFileData(bytes_to_read, fd);
            close(fd);
            chmod(path.c_str(), S_IRWXU);
            return result;
        }
    } catch (std::runtime_error &ex) {
        close(fd);
    }
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

bool TDPService::downloadFileData(size_t &bytes_to_download, int &fd) {
    error_code = NONE;
    last_sent_command = (received_response = "dls\n\n");

    try {
        if (connectionToServer.exchangeWithServer(received_response, bytes_to_download, true, fd))
            return true;
        else
            error_code = E_CLOSED_CON;
    } catch (const std::runtime_error &ex) {
        error_code = E_EXCHANGE_MESSAGE;
    }

    return false;
}