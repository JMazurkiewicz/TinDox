#include "connection.h"
#include <cstring>
#include <system_error>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>


void Connection::createSocket() {
    sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0 );
    if (sock == -1)
        throw std::system_error(errno, std::generic_category(), "creating socket");

    epfd_read = epoll_create1(0);
    epfd_write = epoll_create1(0);
    if (epfd_read == -1 || epfd_write == -1) {
        closeConnection();
        throw std::system_error(errno, std::generic_category(), "creating epoll fd");
    }

    struct epoll_event event_read_sock{}, event_write_sock {};
    memset(&event_read_sock, 0, sizeof(event_read_sock));
    memset(&event_write_sock, 0, sizeof(event_write_sock));
    event_read_sock.data.fd = sock;
    event_write_sock.data.fd = sock;
    event_read_sock.events = EPOLLIN;
    event_write_sock.events = EPOLLOUT;
    if (epoll_ctl(epfd_read, EPOLL_CTL_ADD, sock, &event_read_sock) == -1
    || epoll_ctl(epfd_write, EPOLL_CTL_ADD, sock, &event_write_sock) == -1) {
        closeConnection();
        throw std::system_error(errno, std::generic_category(), "epoll - add");
    }
}

void Connection::connectToServer(const string& serv_ip, int serv_port) {
    createSocket();
    server.sin_family = AF_INET;
    server.sin_port = htons(serv_port);
    server.sin_addr.s_addr = inet_addr(serv_ip.c_str());
    memset(server.sin_zero, '\0', sizeof server.sin_zero);
    isConnectionOpen = true;
    if (connect(sock, (struct sockaddr *) &server, sizeof server) == -1) {
        if (errno != EINPROGRESS) {
            closeConnection();
            throw std::system_error(errno, std::generic_category(), "connecting to server");
        }
    }
}

void Connection::closeConnection() {
    if(isConnectionOpen) {

        if(close(sock) != 0)
            throw std::system_error(errno, std::generic_category(), "closing socket");

        isConnectionOpen = false;
    }
}

bool Connection::sendToServer(std::string &message) {
    if(isConnectionOpen) {
        const char *msg = message.c_str();
        unsigned long len = message.size() + 1;
        unsigned long buf_pos = 0;
        while(len) {
            int new_events = epoll_wait(epfd_write, events, MAX_EPOLL_EVENTS, -1);
            if (new_events == -1) {
                closeConnection();
                throw std::system_error(errno, std::generic_category(), "waiting to send message");
            }
            for (int i = 0; i <new_events; i ++) {
                if ((events[i].events & EPOLLOUT))
                {
                    while (len) {
                        ssize_t sent_bytes = write(sock, msg + buf_pos, len);
                        if(sent_bytes < 0) {
                            if (errno == EAGAIN || errno == EWOULDBLOCK)
                                break;
                            throw std::system_error(errno, std::generic_category(), "sending message");
                        }
                        buf_pos += sent_bytes;
                        len -= sent_bytes;
                    }
                }
            }
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
    memset(buf, '\0', sizeof buf);
    int new_events = epoll_wait(epfd_read, events, MAX_EPOLL_EVENTS, 5000);
    if (new_events == -1) {
        closeConnection();
        delete[] buf;
        throw std::system_error(errno, std::generic_category(), "waiting for response");
    }
    else if (new_events == 0) { //timeout expired
        delete[] buf;
        return false;
    }

    for (int i = 0; i <new_events; i ++) {
        if ((events[i].events & EPOLLIN))
        {
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
        }
    }
    delete[] buf;

    if (message.empty())
        return false;
    else
        return true;
}