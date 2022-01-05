#ifndef TINDOX_CONSOLE_CLIENT_TDPSERVICE_H
#define TINDOX_CONSOLE_CLIENT_TDPSERVICE_H

#include "connection.h"
#include "responseanalyzer.h"

class TDPService {
public:
	TDPService() : connectionToServer(std::make_shared<Connection>()), responseAnalyzer(connectionToServer) {}

	~TDPService() {
		closeConnection();
	}

	bool initConnection(const string &serv_ip, int serv_port);

	bool closeConnection();

	bool logout();

	bool auth(const string &login, const string &passwd);

	bool mkdir(const string &name);

	bool rm(const string &name);

	bool rename(const string &oname, const string &nname);

	bool cp(const string &name, const string &path);

	bool mv(const string &name, const string &path);

	bool cd(const string &path);

	bool tree(const string &path);

	bool ls(const string &path, const string &size, const string &mod);

	bool name();

	bool pwd();

	string response_body;
	string received_response;
	string last_sent_command;
	ConnectionError error_code = NONE;

private:
	shared_ptr<Connection> connectionToServer;
	ResponseAnalyzer responseAnalyzer;

	bool sendCommand(const string &command_name, const string &field_name1, const string &field_value1,
	                 const string &field_name2, const string &field_value2,
	                 const string &field_name3, const string &field_value3);
};


#endif //TINDOX_CONSOLE_CLIENT_TDPSERVICE_H
