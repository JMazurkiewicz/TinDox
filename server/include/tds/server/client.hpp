#pragma once

#include "tds/ip/tcp_socket.hpp"
#include "tds/linux/event_type.hpp"
#include "tds/protocol/client_context.hpp"
#include "tds/protocol/default_command_executor.hpp"
#include "tds/protocol/download_manager.hpp"
#include "tds/protocol/protocol_interpreter.hpp"
#include "tds/protocol/receiver.hpp"
#include "tds/protocol/request.hpp"
#include "tds/protocol/sender.hpp"
#include "tds/protocol/server_context.hpp"
#include "tds/protocol/upload_manager.hpp"

#include <vector>

namespace tds::server {
    class Client {
    public:
        explicit Client(ip::TcpSocket socket, protocol::ServerContext& server_context);
        ~Client();

        Client(const Client&) = delete;
        Client& operator=(const Client&) = delete;

        ip::TcpSocket& get_socket() noexcept;

        bool is_alive() const noexcept;
        linux::EventType get_required_events() const;
        void handle(linux::EventType events);

    private:
        void handle_pending_input();
        void handle_current_input();
        void handle_input(std::span<const char>& input);
        void handle_request(std::span<const char>& input);
        void handle_upload(std::span<const char>& input);

        void execute_command(const protocol::Request& request);
        void start_download();
        void start_upload();

        void handle_output();
        void handle_executor_output();
        void handle_download_manager_output();

        ip::TcpSocket m_socket;
        protocol::ClientContext m_context;

        protocol::Receiver m_receiver;
        std::vector<char> m_pending_input;
        protocol::ProtocolInterpreter m_interpreter;
        protocol::DefaultCommandExecutor m_command_executor;
        protocol::Sender m_sender;

        protocol::DownloadManager m_download_manager;
        protocol::UploadManager m_upload_manager;
    };
}
