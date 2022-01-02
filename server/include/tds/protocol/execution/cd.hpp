#pragma once

#include "tds/protocol/execution/command_base.hpp"
#include "tds/protocol/field.hpp"
#include "tds/user/permissions.hpp"

#include <span>

namespace tds::protocol::execution {
    class Cd : public CommandBase {
    public:
        static constexpr std::string_view name = "cd";
        static constexpr user::Permissions required_perms = user::Permissions::none;
        static constexpr bool requires_authorization = true;

        void parse_fields(std::span<const Field> fields);
        void execute();

    private:
        std::filesystem::path m_next_path;
    };
}
