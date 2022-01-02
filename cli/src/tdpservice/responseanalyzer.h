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

    ConnectionError getFirstLine(string &command_name);

    int getErrorCode(int &buf_pos, string &buf);


};


#endif //TINDOX_CONSOLE_CLIENT_RESPONSEANALYZER_H
