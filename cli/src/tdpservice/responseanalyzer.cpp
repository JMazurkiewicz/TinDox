#include <system_error>
#include "responseanalyzer.h"

ConnectionError ResponseAnalyzer::readSingleLineResponse(string command_name) {

    return NONE;
}

ConnectionError ResponseAnalyzer::getFirstLine(string &command_name) {
    bool foundCorrectLine = false;
    string read_buffer;
    int response_code = 0, buf_pos = 0;
    try {
        if(!connectionToServer.receiveAllReadyFromServer(read_buffer))
            return E_NOTHING_READ;

        while(!foundCorrectLine) {
            response_code = getErrorCode(buf_pos, read_buffer);
        }
    }  catch (const std::system_error& ex) {
        return E_READ_MESSAGE;
    }

    return OK;
}

int ResponseAnalyzer::getErrorCode(int &buf_pos, string &buf) {
    
    return 0;
}
