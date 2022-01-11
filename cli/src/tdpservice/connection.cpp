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

	//epfd_read = epoll_create1(0);
	//epfd_write = epoll_create1(0);
	epfd = epoll_create1(0);
	if (epfd_read == -1 || epfd_write == -1) {
		closeConnection();
		throw std::system_error(errno, std::generic_category(), "creating epoll fd");
	}

	struct epoll_event events = {};
	events.data.fd = sock;
	events.events = EPOLLOUT | EPOLLET;
	epoll_ctl(...);

	//struct epoll_event event_read_sock{}, event_write_sock{};
	//memset(&event_read_sock, 0, sizeof(event_read_sock));
	//memset(&event_write_sock, 0, sizeof(event_write_sock));
	//event_read_sock.data.fd = sock;
	//event_write_sock.data.fd = sock;
	//event_read_sock.events = EPOLLIN;
	//event_write_sock.events = EPOLLOUT;
	//if (epoll_ctl(epfd_read, EPOLL_CTL_ADD, sock, &event_read_sock) == -1
	//    || epoll_ctl(epfd_write, EPOLL_CTL_ADD, sock, &event_write_sock) == -1) {
	//	closeConnection();
	//	throw std::system_error(errno, std::generic_category(), "epoll - add");
	//}
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

		if (close(sock) != 0)
			//throw std::system_error(errno, std::generic_category(), "closing socket");

		isConnectionOpen = false;
	}
}

void Connection::main_loop() {
	epoll_event events[2];
	while(isConnectionOpen) {
		ssize_t count = epoll_wait(epfd, events, 2, -1);
		for(auto& event : events | take(count)) {
			if(event.data.fd == sock) {
				if((event.events & EPOLLIN) == 0) {
					// czytasz całe wejście i przygotowujesz dane do wysłania
				} else if((event.events & EPOLLOUT) == 0) {
					// wysyłasz tyle ile się da
				}
			} else {
				// źle
			}

			epoll_event new_event = {};
			new_event.data.fd = sock;
			new_event.events = EPOLLET;
			if(nie_dostałem_całej_odpowiedzi_od_serwera) {
				new_event.events |= EPOLLIN;
			} else {
				new_event.events |= EPOLLOUT;
			}

			epoll_ctl(epfd, EPOLL_CTL_MOD, sock, &new_event);
		}
	}
}

bool Connection::sendToServer(std::string &message) {
	if (isConnectionOpen) {
		const char *msg = message.c_str();
		unsigned long len = message.size();
		unsigned long buf_pos = 0;
		while (len) {
			int new_events = epoll_wait(epfd_write, events, MAX_EPOLL_EVENTS, -1);
			if (new_events == -1) {
				closeConnection();
				throw std::system_error(errno, std::generic_category(), "waiting to send message");
			}
			for (int i = 0; i < new_events; i++) {
				if ((events[i].events & EPOLLOUT)) {
					while (len) {
						ssize_t sent_bytes = write(sock, msg + buf_pos, len);
						if (sent_bytes < 0) {
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
	if (!isConnectionOpen)
		return false;

	message.clear();
	char buf[BUF_MAX_SIZE];
	///char *buf = new char[BUF_MAX_SIZE];
	memset(buf, '\0', sizeof buf);
	int new_events = epoll_wait(epfd_read, events, MAX_EPOLL_EVENTS, -1);
	if (new_events == -1) {
		closeConnection();
		throw std::system_error(errno, std::generic_category(), "waiting for response");
	} else if (new_events == 0) { //timeout expired
		return false;
	}

	for (int i = 0; i < new_events; i++) {
		if ((events[i].events & EPOLLIN)) {
			while (true) {
				ssize_t read_bytes = read(sock, buf, BUF_MAX_SIZE - 1);
				if (read_bytes == -1) {
					if (errno == EAGAIN || errno == EWOULDBLOCK) {
					ś	break;
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

			if(message.ends_with("\n\n")) {
				// oddajesz do analizatora
				nie_dostałem_całej_odpowiedzi_od_serwera = false;
			} else {
				// czekamy na kolejne bajty
			}

			//switch()
			//	// czy wysyłam komendy, czy pobieram plik

				// CHECK: czy dwa ostatnie znaki to '\n'
				// dl
				// name:
				// dls
				// OSTRZEŻENIE DLA UŻYTKOWNIKA -- zgłosił do @JMaz
		}
	}

	if (message.empty())
		return false;
	else
		return true;
}
