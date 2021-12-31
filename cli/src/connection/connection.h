#ifndef TINDOX_CONSOLE_CLIENT_CONNECTION_H
#define TINDOX_CONSOLE_CLIENT_CONNECTION_H

#include <string>
#include <netdb.h>
#include <sys/epoll.h>

#define MAX_EPOLL_EVENTS 64
#define BUF_SIZE 2048

using std::string;

class Connection {
public:

    Connection() = default;

    ~Connection() {
        closeConnection();
    }

    void connectToServer(const string& serv_ip, int serv_port);

    void closeConnection();

    bool sendToServer(std::string &message) const;

    bool receiveAllReadyFromServer(std::string &message);


    bool isOpenedConnection() const {
        return isConnectionOpen;
    }

private:

    void createSocket();

    int sock, epfd;
    bool isConnectionOpen = false;
    struct sockaddr_in server;
    struct epoll_event events[MAX_EPOLL_EVENTS];
};


#endif //TINDOX_CONSOLE_CLIENT_CONNECTION_H
