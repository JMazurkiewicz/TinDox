#include "connection.h"
#include <cstring>
#include <system_error>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>


void Connection::createSocket() {
    sock = socket( AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0 );
    if (sock == -1)
        throw std::system_error(errno, std::generic_category(), "creating socket");

    epfd = epoll_create1(0);
    if (epfd == -1)
        throw std::system_error(errno, std::generic_category(), "creating epoll fd");

    struct epoll_event event{};
    memset(&event, 0, sizeof(event));
    event.data.fd = sock;
    event.events = EPOLLIN | EPOLLET;               //edge-triggered - on epoll_wait only put process to sleep
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &event) == -1)
        throw std::system_error(errno, std::generic_category(), "epoll - add");
}

void Connection::connectToServer(const string& serv_ip, int serv_port) {
    createSocket();
    server.sin_family = AF_INET;
    server.sin_port = htons(serv_port);
    server.sin_addr.s_addr = inet_addr(serv_ip.c_str());
    memset(server.sin_zero, '\0', sizeof server.sin_zero);
    isConnectionOpen = true;
    if (connect(sock, (struct sockaddr *) &server, sizeof server) == -1) {
        if(errno != EINPROGRESS) {
            closeConnection();
            throw std::system_error(errno, std::generic_category(), "connecting to server");
        }
    }

    if(epoll_wait(epfd, events, MAX_EPOLL_EVENTS, 10000) == -1) {
        closeConnection();
        throw std::system_error(errno, std::generic_category(), "waiting for connection");
    }
}

void Connection::closeConnection() {
    if(isConnectionOpen) {

        if (epoll_ctl(epfd, EPOLL_CTL_DEL, sock, nullptr) == -1)
            throw std::system_error(errno, std::generic_category(), "epoll - delete");

        if(close(sock) != 0)
            throw std::system_error(errno, std::generic_category(), "closing socket");

        isConnectionOpen = false;
    }
}

bool Connection::sendToServer(std::string &message) const {
    if(isConnectionOpen) {
        const char *msg = message.c_str();
        unsigned long len = message.size() + 1;
        unsigned long buf_pos = 0;
        while(len) {
            ssize_t sent_bytes = write(sock, msg + buf_pos, len);
            if(sent_bytes < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    continue;
                throw std::system_error(errno, std::generic_category(), "sending message");
            }

            buf_pos += sent_bytes;
            len -= sent_bytes;
        }
    } else
        return false;

    return true;
}

bool Connection::receiveAllReadyFromServer(std::string &message) {
    if(!isConnectionOpen)
        return false;

    message.clear();
    char *buf = new char[BUF_SIZE + 1];

//    int new_events = epoll_wait(epfd, events, MAX_EPOLL_EVENTS, 5000);
//    if (new_events == -1) {
//        closeConnection();
//        delete[] buf;
//        throw std::system_error(errno, std::generic_category(), "waiting for response");
//    }
//    else if (new_events == 0) { //timeout expired
//        delete[] buf;
//        return false;
//    }

//    for (int i = 0; i <new_events; i ++) {
//        if ((events [i] .events & EPOLLIN))
//        {
            while (true) {
                ssize_t read_bytes = read(sock, buf, BUF_SIZE);
                if (read_bytes == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        break;
                    } else {
                        delete[] buf;
                        closeConnection();
                        throw std::system_error(errno, std::generic_category(), "reading message");
                    }
                } else if (read_bytes == 0) { //EOF -> connection closed
                    delete[] buf;
                    closeConnection();
                    return false;
                }
                message.append(buf);
            }
//        }
//    }

    delete[] buf;
    return true;
}