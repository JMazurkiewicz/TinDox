#include "tds/cli/run_command.hpp"

#include "tds/linux/linux_error.hpp"

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
        m_signals.add_handler(SIGPIPE, handler);
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
        while(m_continue) {
            m_epoll.handle();
        }
    }

    void RunCommand::handle_signal(int code) {
        if(code == SIGINT) {
            std::cout << "Got SIGINT\n";
        } else if(code == SIGQUIT) {
            std::cout << "Got SIGQUIT\n";
        } else if(code == SIGABRT) {
            std::cout << "Got SIGABRT\n";
        } else if(code == SIGTERM) {
            std::cout << "Got SIGTERM\n";
        } else if(code == SIGPIPE) {
            std::cout << "Got SIGPIPE\n";
            return;
        }

        m_continue = false;
    }

    void RunCommand::handle_connection(int fd, ip::EndpointV4 client) {
        std::cout << "Got new connection from " << client << " [" << fd << "], sending 'hello' message" << std::endl;

        std::string hello = "Hello " + to_string(client) + "!\n";
        write(fd, hello.data(), hello.size());

        auto connection = std::make_unique<Connection>(fd);
        connection->client = client;
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
        std::array<char, 256> buffer;
        const auto amount = read(get_fd(), buffer.data(), buffer.size());
        if(amount == -1) {
            throw linux::LinuxError{"read(2)"};
        } else {
            std::string_view msg{buffer.data(), static_cast<size_t>(amount)};
            std::cout << msg << std::flush;
            const auto answer = '[' + to_string(client) + "]: " + std::string{msg};

            ssize_t written = 0;
            do {
                const int bytes = write(get_fd(), answer.data() + written, answer.size() - written);

                if(bytes == -1) {
                    throw linux::LinuxError{"write(2)"};
                } else {
                    written += bytes;
                }
            } while(written != answer.size());
        }
    }
}
