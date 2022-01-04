#include "tdpservice.h"
#include <sstream>

bool TDPService::initConnection(const string &serv_ip, int serv_port) {
	try {
		connectionToServer->connectToServer(serv_ip, serv_port);
	} catch (const std::system_error &ex) {
		error_code = E_CON_TO_SERV;
		return false;
	}

	return true;
}

bool TDPService::closeConnection() {

	if (sendCommand("exit", "", "", "", "", "", "")) {
		try {
			connectionToServer->closeConnection();
			return true;
		} catch (const std::system_error &ex) {
			error_code = E_CLOSE_CON;
		}
	}
	return false;
}

bool TDPService::logout() {
	if (sendCommand("logout", "", "", "", "", "", "")
	    && (error_code = responseAnalyzer.readSimpleResponse("logout", received_response, response_body, false)) == OK)
		return true;
	else
		return false;
}

bool TDPService::auth(const string &login, const string &passwd) {
	if (sendCommand("auth", "login", login, "passwd", passwd, "", "")
	    && (error_code = responseAnalyzer.readSimpleResponse("auth", received_response, response_body, false)) == OK)
		return true;
	else
		return false;
}

bool TDPService::mkdir(const string &name) {
	if (sendCommand("mkdir", "name", name, "", "", "", "")
	    && (error_code = responseAnalyzer.readSimpleResponse("mkdir", received_response, response_body, false)) == OK)
		return true;
	else
		return false;
}

bool TDPService::rm(const string &name) {
	if (sendCommand("rm", "name", name, "", "", "", "")
	    && (error_code = responseAnalyzer.readSimpleResponse("rm", received_response, response_body, false)) == OK)
		return true;
	else
		return false;
}

bool TDPService::rename(const string &oname, const string &nname) {
	if (sendCommand("rename", "oname", oname, "nname", nname, "", "")
	    && (error_code = responseAnalyzer.readSimpleResponse("rename", received_response, response_body, false)) == OK)
		return true;
	else
		return false;
}

bool TDPService::cp(const string &name, const string &path) {
	if (sendCommand("cp", "name", name, "path", path, "", "")
	    && (error_code = responseAnalyzer.readSimpleResponse("cp", received_response, response_body, false)) == OK)
		return true;
	else
		return false;
}

bool TDPService::mv(const string &name, const string &path) {
	if (sendCommand("mv", "name", name, "path", path, "", "")
	    && (error_code = responseAnalyzer.readSimpleResponse("mv", received_response, response_body, false)) == OK)
		return true;
	else
		return false;
}

bool TDPService::cd(const string &path) {
	if (sendCommand("cd", "path", path, "", "", "", "")
	    && (error_code = responseAnalyzer.readSimpleResponse("cd", received_response, response_body, true)) == OK)
		return true;
	else
		return false;
}

bool TDPService::ls(const string &path, const string &size, const string &mod) {
	if (sendCommand("ls", path.empty() ? "" : "path", path,
	                size.empty() ? "" : "size", size, mod.empty() ? "" : "mod", mod)
	    && (error_code = responseAnalyzer.readLsResponse(received_response, response_body)) == OK)
		return true;
	else
		return false;
}

bool TDPService::name() {
	if (sendCommand("name", "", "", "", "", "", "")
	    && (error_code = responseAnalyzer.readSimpleResponse("name", received_response, response_body, true)) == OK)
		return true;
	else
		return false;
}

bool TDPService::pwd() {
	if (sendCommand("pwd", "", "", "", "", "", "")
	    && (error_code = responseAnalyzer.readSimpleResponse("pwd", received_response, response_body, true)) == OK)
		return true;
	else
		return false;
}

bool TDPService::sendCommand(const string &command_name, const string &field_name1, const string &field_value1,
                             const string &field_name2, const string &field_value2, const string &field_name3,
                             const string &field_value3) {

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

	if (!field_name1.empty())

		last_sent_command += field_name3 + ":" + field_value3 + "\n";

	last_sent_command += "\n";

	try {
		if (connectionToServer->sendToServer(last_sent_command))
			return true;
		else
			error_code = E_CLOSED_CON;
	} catch (const std::system_error &ex) {
		error_code = E_SEND_MESSAGE;
	}

	return false;
}
