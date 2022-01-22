#include "connection.h"
#include <cstring>
#include <system_error>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

void Connection::createSocket() {
    sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (sock == -1)
        throw std::system_error(errno, std::generic_category(), "creating socket");

    epfd = epoll_create1(0);
    if (epfd == -1) {
        closeConnection();
        throw std::system_error(errno, std::generic_category(), "creating epoll fd");
    }

    struct epoll_event events = {};
    events.data.fd = sock;
    events.events = EPOLLOUT | EPOLLET;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &events) == -1) {
        closeConnection();
        throw std::system_error(errno, std::generic_category(), "epoll - add");
    }
}

void Connection::connectToServer(const string &serv_ip, int serv_port) {
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
    if (isConnectionOpen) {
        close(sock);
        isConnectionOpen = false;
    }
}

bool Connection::exchangeWithServer(std::string &data) {
    if (isConnectionOpen) {
        bool gotWholeResponse = false, readyForReading = false;
        unsigned long sent_buf_pos = 0;
        epoll_event events[2];
        while (!gotWholeResponse) {
            ssize_t count = epoll_wait(epfd, events, 2, -1);
            if (count == -1) {
                closeConnection();
                throw std::system_error(errno, std::generic_category(), "waiting on epoll");
            }

            for (int i = 0; i < count; i++) {
                if (events[i].data.fd == sock) {
                    if (events[i].events & EPOLLOUT) {
                        if (sendToServer(data, sent_buf_pos)) {
                            readyForReading = true;
                            data.clear();
                        }
                    } else if (events[i].events & EPOLLIN) {
                        if (!receiveAllReadyFromServer(data, gotWholeResponse))
                            return false;
                        readyForReading = !gotWholeResponse;
                    }
                } else {
                    closeConnection();
                    throw std::runtime_error("epoll - wrong event");
                }

                changeEpollEvents(readyForReading);
            }
        }
    } else
        return false;

    return true;
}

bool Connection::sendToServer(std::string &message, unsigned long &sent_buf_pos) const {
    const char *msg = message.c_str();
    unsigned long len = message.size() - sent_buf_pos;

    while (len) {
        ssize_t sent_bytes = write(sock, msg + sent_buf_pos, len);
        if (sent_bytes < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            throw std::system_error(errno, std::generic_category(), "sending message");
        }
        sent_buf_pos += sent_bytes;
        len -= sent_bytes;
    }

    if (!len)
        return true;
    else
        return false;
}

bool Connection::receiveAllReadyFromServer(std::string &message, bool &gotWholeResponse) {
    char buf[BUF_MAX_SIZE];

    while (true) {
        memset(buf, '\0', sizeof buf);
        ssize_t read_bytes = read(sock, buf, BUF_MAX_SIZE - 1);
        if (read_bytes == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            } else {
                closeConnection();
                throw std::system_error(errno, std::generic_category(), "reading message");
            }
        } else if (read_bytes == 0) { //EOF -> connection closed
            closeConnection();
            return false;
        }

        message.append(buf);
    }

    gotWholeResponse = message.ends_with("\n\n");

    return true;
}

void Connection::changeEpollEvents(bool &readyForReading) {
    epoll_event new_events = {};
    new_events.data.fd = sock;
    new_events.events = EPOLLET;
    if (readyForReading) {
        new_events.events |= EPOLLIN;
    } else {
        new_events.events |= EPOLLOUT;
    }

    if (epoll_ctl(epfd, EPOLL_CTL_MOD, sock, &new_events) == -1) {
        closeConnection();
        throw std::system_error(errno, std::generic_category(), "epoll - add");
    }
}
