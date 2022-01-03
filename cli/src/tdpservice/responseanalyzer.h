#ifndef TINDOX_CONSOLE_CLIENT_RESPONSEANALYZER_H
#define TINDOX_CONSOLE_CLIENT_RESPONSEANALYZER_H

#include "connection.h"
#include "connectionerror.h"

enum ServerResponseCode {

};

class ResponseAnalyzer {
public:
    ResponseAnalyzer(Connection &connec) :connectionToServer(connec) {}

    ConnectionError readSingleLineResponse(string command_name);

private:
    Connection connectionToServer;
    string read_buf;

    ConnectionError getFirstLine(string::iterator &iter, string &command_name);

    ConnectionError getErrorCode(string::iterator &iter);

    ConnectionError readRestIntoBuf();

    ConnectionError skipToNextCommand(string::iterator &iter);

    ConnectionError checkRestOfFirstLine(string::iterator &iter, string command_name);

    ConnectionError nextReadChar(string::iterator &iter);
};


#endif //TINDOX_CONSOLE_CLIENT_RESPONSEANALYZER_H
