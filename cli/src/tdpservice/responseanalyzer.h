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

	ConnectionError readSimpleResponse(string command_name, string &received_response, string &response_body,
	                                   bool isSpecificAnswerExpected);

private:
	string read_buf;
	shared_ptr<Connection> connectionToServer;

	ConnectionError getResponseHeader(string::iterator &iter, string &command_name);

	ConnectionError getErrorCode(string::iterator &iter);

	ConnectionError readRestIntoBuf(string::iterator &iter);

	ConnectionError checkRestOfResponseHeader(string::iterator &iter, string command_name);

	ConnectionError nextReadChar(string::iterator &iter);

	ConnectionError getResponseLine(string::iterator &iter, string &response_body);
};


#endif //TINDOX_CONSOLE_CLIENT_RESPONSEANALYZER_H
