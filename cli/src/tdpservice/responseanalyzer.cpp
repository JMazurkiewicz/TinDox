#include <system_error>
#include "responseanalyzer.h"

ConnectionError ResponseAnalyzer::readSimpleResponse(string command_name, string &received_response,
                                                     string &response_body, bool isSpecificAnswerExpected) {
	auto iter = read_buf.begin();
	received_response.clear();
	ConnectionError response_code = getResponseHeader(iter, command_name);
	if (response_code != E_RESPONSE && response_code != E_NOTHING_READ && response_code != E_READ_MESSAGE) {
		if (response_code == OK && !isSpecificAnswerExpected) {
			if (*iter == '\n') {
				response_body = "";
				received_response = std::to_string(response_code) + " " + command_name + "\n\n";
			} else
				response_code = E_RESPONSE;
		} else {
			if (getResponseLine(iter, response_body) == NONE) {
				received_response = std::to_string(response_code) + " " + command_name + "\n";
				received_response += response_body;
			} else {
				response_body.clear();
				response_code = E_RESPONSE;
			}
		}
	}

	return response_code;
}

ConnectionError
ResponseAnalyzer::readMultilineResponse(string &received_response, string &response_body, string command_name) {
	auto iter = read_buf.begin();
	received_response.clear();
	ConnectionError response_code = getResponseHeader(iter, command_name);
	if (response_code != E_RESPONSE && response_code != E_NOTHING_READ && response_code != E_READ_MESSAGE) {
		if (getMultipleLineResponse(iter, response_body) == NONE) {
			received_response = std::to_string(response_code) + " " + command_name + "\n";
			received_response += response_body;
		} else
			return E_RESPONSE;
	}

	return response_code;
}

ConnectionError ResponseAnalyzer::getResponseHeader(string::iterator &iter, string &command_name) {
	ConnectionError response_code = readRestIntoBuf(iter);
	if (response_code == NONE) {
		response_code = getErrorCode(iter);
		if (response_code != E_RESPONSE) {
			if (checkRestOfResponseHeader(iter, command_name) != NONE)
				return E_RESPONSE;
		}
	}
	return response_code;
}

ConnectionError ResponseAnalyzer::getErrorCode(string::iterator &iter) {
	int code = 0, code_length = 0;
	while (code_length != 3) {
		if (!std::isdigit(*iter))
			return E_RESPONSE;
		else {
			code = 10 * code + (*iter - '0');
			++code_length;
		}

		if (nextReadChar(iter) != NONE)
			return E_RESPONSE;
	}

	if (code == 100 || (code >= 300 && code <= 303) || (code >= 401 && code <= 409))
		return (ConnectionError) code;
	else
		return E_RESPONSE;
}

ConnectionError ResponseAnalyzer::readRestIntoBuf(string::iterator &iter) {
	try {
		if (connectionToServer->receiveAllReadyFromServer(read_buf)) {
			iter = read_buf.begin();
			return NONE;
		} else
			return E_NOTHING_READ;
	} catch (const std::system_error &ex) {
		return E_READ_MESSAGE;
	}
}

ConnectionError ResponseAnalyzer::checkRestOfResponseHeader(string::iterator &iter, const string &command_name) {

	const string correct_ending = " " + command_name + "\n";
	for (char i: correct_ending) {
		if (*iter != i)
			return E_BAD_COMMAND;

		if (nextReadChar(iter) != NONE)
			return E_RESPONSE;
	}

	return NONE;
}

ConnectionError ResponseAnalyzer::nextReadChar(string::iterator &iter) {
	if (iter == read_buf.end())
		return E_RESPONSE;
	++iter;
	if (iter == read_buf.end())
		return E_RESPONSE;
	else
		return NONE;
}

ConnectionError ResponseAnalyzer::getResponseLine(string::iterator &iter, string &response_body) {
	response_body.clear();

	if (*iter == '\n')
		return E_RESPONSE;

	bool found_new_line_char = false;
	while (!found_new_line_char) {
		found_new_line_char = (*iter == '\n');
		response_body += *iter;
		if (nextReadChar(iter) != NONE)
			return E_RESPONSE;
	}

	if (*iter == '\n') {
		response_body += '\n';
		return NONE;
	} else
		return E_RESPONSE;
}

ConnectionError ResponseAnalyzer::getMultipleLineResponse(string::iterator &iter, string &response_body) {
	response_body.clear();

	if (*iter != '\n') {
		bool found_new_line_char, found_command_separator = false;

		while (!found_command_separator) {
			found_new_line_char = false;

			while (!found_new_line_char) {
				found_new_line_char = (*iter == '\n');
				response_body += *iter;
				if (nextReadChar(iter) != NONE)
					return E_RESPONSE;
			}

			found_command_separator = (*iter == '\n');
		}
	}
	response_body += "\n";
	return NONE;
}
