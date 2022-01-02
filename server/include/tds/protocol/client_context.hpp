#pragma once

#include "tds/protocol/auth_token.hpp"
#include "tds/protocol/protocol_interpreter.hpp"
#include "tds/protocol/protocol_mode.hpp"
#include "tds/protocol/server_context.hpp"
#include "tds/user/permissions.hpp"

#include <filesystem>
#include <optional>

namespace tds::protocol {
    class ClientContext {
    public:
        explicit ClientContext();
        ClientContext(const ClientContext&) = delete;
        ClientContext& operator=(const ClientContext&) = delete;

        bool is_alive() const noexcept;
        void kill();

        ProtocolMode get_mode() const noexcept;
        void set_mode(ProtocolMode mode);

        bool is_authorized() const noexcept;
        void set_new_token(AuthToken new_token);
        const AuthToken& get_auth_token() const noexcept;
        void logout();

        const std::filesystem::path& get_current_path();
        void set_current_path(std::filesystem::path path);

    private:
        bool m_alive;
        ProtocolMode m_mode;
        std::optional<AuthToken> m_token;
        std::filesystem::path m_current_path;
    };
}
