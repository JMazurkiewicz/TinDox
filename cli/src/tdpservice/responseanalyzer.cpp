#include <system_error>
#include "responseanalyzer.h"

ConnectionError ResponseAnalyzer::readSingleLineResponse(string command_name) {
    auto iter = read_buf.begin();

    while(true) {
        ConnectionError response_code = getFirstLine(iter, command_name);
        if(response_code == E_RESPONSE || response_code == E_NOTHING_READ || response_code == E_READ_MESSAGE)
            return response_code;

        if(nextReadChar(iter) != NONE)
            return E_RESPONSE;

        if(*iter == '\n')
            return response_code;
        else if (skipToNextCommand(iter) != NONE)
            return E_RESPONSE;
    }
}

ConnectionError ResponseAnalyzer::getFirstLine(string::iterator &iter, string &command_name) {
    ConnectionError response_code = readRestIntoBuf(iter);
    if(response_code != NONE)
        return response_code;

    while(true) {
        response_code = getErrorCode(iter);
        if(response_code == E_RESPONSE)
            break;

        ConnectionError temp = checkRestOfFirstLine(iter, command_name);
        if(temp == E_RESPONSE)
            return E_RESPONSE;
        else if(temp == NONE)
            break;
    }

    return response_code;
}

ConnectionError ResponseAnalyzer::getErrorCode(string::iterator &iter) {
    while(true) {
        int code = 0, code_length = 0;
        while (code_length != 3) {
            if (!std::isdigit(*iter)) {
                code_length = 0;
                if (skipToNextCommand(iter) != NONE)
                    return E_RESPONSE;
                continue;
            } else {
                code = 10 * code + (*iter - '0');
                ++code_length;
            }
            if (nextReadChar(iter) != NONE)
                return E_RESPONSE;
        }

        if(code == 100 || (code >= 300 && code <= 303) || (code >= 401 && code <= 409))
            return (ConnectionError) code;
        else if (skipToNextCommand(iter) != NONE)
            return E_RESPONSE;
    }
}

ConnectionError ResponseAnalyzer::readRestIntoBuf(string::iterator &iter) {
    try {
        if(connectionToServer->receiveAllReadyFromServer(read_buf)) {
            iter = read_buf.begin();
            return NONE;
        }
        else
            return E_NOTHING_READ;
    }  catch (const std::system_error& ex) {
        return E_READ_MESSAGE;
    }
}

ConnectionError ResponseAnalyzer::skipToNextCommand(string::iterator &iter) {
    bool found_command_separator = false;
    while(!found_command_separator) {
        bool found_new_line_char = false;
        while (!found_new_line_char) {
            found_new_line_char = (*iter == '\n');
            if (nextReadChar(iter) != NONE)
                return E_RESPONSE;
        }
        found_command_separator = (*iter == '\n');
    }

    while(*iter == '\n') {  //skipping all '\n'
        if (nextReadChar(iter) != NONE)
            return E_RESPONSE;
    }

    return NONE;
}

ConnectionError ResponseAnalyzer::checkRestOfFirstLine(string::iterator &iter, string command_name) {

    const string correct_ending = " " + command_name + "\n";
    for(int i = 0; i < correct_ending.size(); ++i) {
        if (*iter != correct_ending[i])
            return E_BAD_COMMAND;

        if (nextReadChar(iter) != NONE)
                return E_RESPONSE;
    }

    return NONE;
}

ConnectionError ResponseAnalyzer::nextReadChar(string::iterator &iter) {
    ++iter;
    if (iter == read_buf.end()) {
        if (readRestIntoBuf(iter) != NONE)
            return E_RESPONSE;
    }
    return NONE;
}
