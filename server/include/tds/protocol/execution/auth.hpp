#pragma once

#include "tds/protocol/execution/command_base.hpp"

#include <optional>

namespace tds::protocol::execution {
    class Auth : public CommandBase {
    public:
        static constexpr int max_try_count = 3;
        static constexpr std::string_view name = "auth";
        static constexpr user::Permissions required_perms = user::Permissions::none;
        static constexpr bool requires_authorization = false;

        void parse_fields(std::span<const Field> fields);
        void execute();

    private:
        void parse_username(const Field& login_field);
        void parse_password(const Field& passwd_field);

        std::optional<std::string> m_username;
        std::optional<std::string> m_password;
    };
}
