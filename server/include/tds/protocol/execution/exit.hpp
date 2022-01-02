#pragma once

#include "tds/protocol/execution/command_base.hpp"

namespace tds::protocol::execution {
    class Exit : public CommandBase {
    public:
        static constexpr std::string_view name = "exit";
        static constexpr user::Permissions required_perms = user::Permissions::none;
        static constexpr bool requires_authorization = false;

        void parse_fields(std::span<const Field> fields);
        void execute();
    };
}
