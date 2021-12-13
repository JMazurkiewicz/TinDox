#ifndef TINDOX_CONSOLE_CLIENT_CONNECTION_H
#define TINDOX_CONSOLE_CLIENT_CONNECTION_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>

using std::string;

class Connection {
public:

    Connection() {}

    ~Connection() {
        closeConnection();
    }

    void connectToServer(string serv_ip, int serv_port);

    void closeConnection();

    bool sendToServer(std::string &message);

    bool receiveFromServer(std::string &message, int len);


    bool isOpenedConnection() {
        return isConnectionOpen;
    }

private:

    void createSocket();

    int sock;
    bool isConnectionOpen = false;
    struct sockaddr_in server;
};


#endif //TINDOX_CONSOLE_CLIENT_CONNECTION_H
