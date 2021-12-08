#pragma once

#include "tds/cli/command_interface.hpp"
#include "tds/ip/endpoint_v4.hpp"
#include "tds/ip/tcp_listener.hpp"
#include "tds/linux/epoll_device.hpp"
#include "tds/linux/fd_buf.hpp"
#include "tds/linux/io_device.hpp"
#include "tds/linux/linux_error.hpp"
#include "tds/linux/signal_device.hpp"

#include <memory>
#include <vector>

namespace tds::cli {
    class RunCommand : public CommandInterface<RunCommand> {
    public:
        static constexpr std::string_view name = "run";

        RunCommand();

        void parse_arguments(std::span<const std::string_view> args);
        void execute();

    private:
        void configure_signals();
        void configure_listener();
        void configure_epoll();
        void start_epoll();

        void handle_signal(int code);
        void handle_connection(int fd, ip::EndpointV4 client);

        void handle_system_error(const std::system_error& e);
        void handle_exception(const std::exception& e);

        linux::EpollDevice m_epoll;
        linux::SignalDevice m_signals;
        ip::TcpListener m_listener;

        bool m_continue;

        struct Connection : public linux::IoDevice {
            ip::EndpointV4 client;

            using IoDevice::IoDevice;
            ~Connection() override;
            void handle() override;
        };

        std::vector<std::unique_ptr<Connection>> m_connections;
    };
}
