#include <cstring>
#include "connection.h"

void Connection::createSocket() {
    sock = socket( AF_INET, SOCK_STREAM, 0 );
    if (sock == -1) {
        perror("opening socket");
        exit(1);
    }
}

void Connection::connectToServer(string serv_ip, int serv_port) {
        createSocket();
        server.sin_family = AF_INET;
        server.sin_port = htons(serv_port);
        server.sin_addr.s_addr = inet_addr(serv_ip.c_str());
        memset(server.sin_zero, '\0', sizeof server.sin_zero);

        if (connect(sock, (struct sockaddr *) &server, sizeof server) == -1) {
            perror("connecting to server");
            exit(1);
        }

        isConnectionOpen = true;
}

void Connection::closeConnection() {
    if(isConnectionOpen) {
        if(close(sock) != 0) {
            perror("closing socket");
            exit(1);
        }

        isConnectionOpen = false;
    }
}

bool Connection::sendToServer(std::string &message) {
    if(isConnectionOpen) {
        const char *msg = message.c_str();
        int len = message.size() + 1;
        int sent_bytes, buf_pos = 0;
        while(len) {
            sent_bytes = write(sock, msg + buf_pos, len);
            if(sent_bytes < 0) {
                perror("sending message");
                exit(1);
            }
            buf_pos += sent_bytes;
            len -= sent_bytes;
        }
    }
    else {
        return false;
    }

    return true;
}

bool Connection::receiveFromServer(std::string &message, int len) {
    if(!isConnectionOpen)
        return false;

    message.clear();
    char *buf = new char[len + 1];
    int read_bytes = read(sock, buf, len);
    if (read_bytes > len) {
        delete [] buf;
        return false;
    }
    buf[read_bytes] = '\0';
    if(read_bytes < 0) {
        perror("reading message");
        delete []buf;
        exit(-1);
    }
    message.append(buf);

    delete []buf;

    if(message.empty()) {   //connection lost
        closeConnection();
        return false;
    }

    return true;
}