#include "tds/cli/run_command.hpp"

#include "tds/linux/epoll_buffer.hpp"
#include "tds/linux/linux_error.hpp"

#include <algorithm>
#include <csignal>
#include <iostream>

namespace tds::cli {
    RunCommand::RunCommand()
        : m_continue{true} { }

    void RunCommand::parse_arguments(std::span<const std::string_view>) { }

    void RunCommand::execute() {
        try {
            execute_steps(&RunCommand::configure_signals, &RunCommand::configure_listener, &RunCommand::configure_epoll,
                          &RunCommand::start_epoll);
        } catch(const std::system_error& e) {
            handle_system_error(e);
        } catch(const std::exception& e) {
            handle_exception(e);
        }
    }

    void RunCommand::configure_signals() {
        auto handler = std::bind_front(&RunCommand::handle_signal, this);
        m_signals.add_handler(SIGINT, handler);
        m_signals.add_handler(SIGQUIT, handler);
        m_signals.add_handler(SIGABRT, handler);
        m_signals.add_handler(SIGTERM, handler);
        m_signals.apply();
    }

    void RunCommand::configure_listener() {
        m_listener.listen(ip::Port::default_port);
        m_listener.set_connection_handler(std::bind_front(&RunCommand::handle_connection, this));
    }

    void RunCommand::configure_epoll() {
        m_epoll.add_device(m_signals);
        m_epoll.add_device(m_listener);
    }

    void RunCommand::start_epoll() {
        linux::EpollBuffer epoll_buffer{32};
        while(m_continue) {
            m_epoll.wait_for_events(epoll_buffer);
            for(int fd : epoll_buffer.get_available_events()) {
                if(fd == m_signals.get_fd()) {
                    m_signals.handle_last_signal();
                } else if(fd == m_listener.get_fd()) {
                    m_listener.handle_last_connection();
                } else {
                    auto connection = std::ranges::find(m_connections, fd, [](auto& ptr) { return ptr->get_fd(); });
                    (*connection)->handle();
                }
            }

            std::erase_if(m_connections, [](auto& connection) { return !connection->is_valid(); });
        }
    }

    void RunCommand::handle_signal(int code) {
        std::cout << "Got ";
        if(code == SIGINT) {
            std::cout << "SIGINT";
        } else if(code == SIGQUIT) {
            std::cout << "SIGQUIT";
        } else if(code == SIGABRT) {
            std::cout << "SIGABRT";
        } else if(code == SIGTERM) {
            std::cout << "SIGTERM";
        }

        std::cout << std::endl;
        m_continue = false;
    }

    void RunCommand::handle_connection(int fd, ip::EndpointV4 client) {
        std::cout << "Got new connection from " << client << " [" << fd << "], sending 'hello' message" << std::endl;

        std::string hello = "Hello " + to_string(client) + "!\n";
        write(fd, hello.data(), hello.size());

        auto connection = std::make_unique<Connection>(fd);
        connection->client = client;
        connection->parent = this;
        m_epoll.add_device(*connection);
        m_connections.emplace_back(std::move(connection));
    }

    void RunCommand::handle_system_error(const std::system_error& e) {
        log_error() << "error: " << e.what() << "\ncode: " << e.code() << '\n';
    }

    void RunCommand::handle_exception(const std::exception& e) {
        log_error() << "error: " << e.what() << '\n';
    }

    RunCommand::Connection::~Connection() {
        std::cout << "Bye " << client << "!\n";
    }

    void RunCommand::Connection::handle() {
        try {
            std::array<char, 256> buffer;
            const auto amount = read(buffer.data(), buffer.size());
            if(amount == 0) {
                throw std::runtime_error{"Connection lost"};
            }

            std::string_view msg{buffer.data(), static_cast<size_t>(amount)};
            std::cout << '[' << client << "]: " << msg << std::flush;

            ssize_t written = 0;
            do {
                const int bytes = write(msg.data() + written, msg.size() - written);
                written += bytes;
            } while(written != msg.size());
        } catch(const std::exception& e) {
            std::cerr << "error: '" << e.what() << "'\n";
            parent->m_epoll.remove_device(*this);
            close();
            set_fd(-1);
        }
    }
}
