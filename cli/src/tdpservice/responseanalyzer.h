#ifndef TINDOX_CONSOLE_CLIENT_RESPONSEANALYZER_H
#define TINDOX_CONSOLE_CLIENT_RESPONSEANALYZER_H

#include "connection.h"
#include "connectionerror.h"
#include <memory>
#include <utility>

using std::shared_ptr;

class ResponseAnalyzer {
public:
    ResponseAnalyzer(shared_ptr<Connection> conn) : connectionToServer(std::move(conn)) {}

    ConnectionError readSingleLineResponse(string command_name);

private:
    string read_buf;
    shared_ptr<Connection> connectionToServer;

    ConnectionError getFirstLine(string::iterator &iter, string &command_name);

    ConnectionError getErrorCode(string::iterator &iter);

    ConnectionError readRestIntoBuf(string::iterator &iter);

    ConnectionError skipToNextCommand(string::iterator &iter);

    ConnectionError checkRestOfFirstLine(string::iterator &iter, string command_name);

    ConnectionError nextReadChar(string::iterator &iter);
};


#endif //TINDOX_CONSOLE_CLIENT_RESPONSEANALYZER_H
