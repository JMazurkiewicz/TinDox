#ifndef TINDOX_CONSOLE_CLIENT_CONNECTION_H
#define TINDOX_CONSOLE_CLIENT_CONNECTION_H

#include <string>
#include <netdb.h>
#include <sys/epoll.h>
#include <thread>

#define BUF_MAX_SIZE 2048

using std::string, std::thread;
typedef unsigned long long ull;

class Connection {
public:

    Connection() = default;

    ~Connection() {
        closeConnection();
    }

    void connectToServer(const string &serv_ip, int serv_port);

    void closeConnection();

    bool exchangeWithServer(string &data, size_t bytes_to_read = 0, bool downloadingFile = false, int fd = -1);

    bool uploadFile(int &fd, off_t &offset, const size_t &size);

private:

    int sock{}, epfd{};
    thread dl_ul_thread;  //TODO jak to połączyć z TUI?
    bool isConnectionOpen = false;
    struct sockaddr_in server{};

    void createSocket();

    bool sendToServer(std::string &message, unsigned long &sent_buf_pos) const;

    bool receiveAllReadyFromServer(std::string &message, bool &gotWholeResponse, size_t &bytes_to_read,
                                   bool readFile = false, int fd = -1);

    void changeEpollEvents(bool &readyForReading);
};


#endif //TINDOX_CONSOLE_CLIENT_CONNECTION_H
