#pragma once

#include "tds/protocol/execution/path_based_command.hpp"

namespace tds::protocol::execution {
    class Tree : public PathBasedCommand {
    public:
        static constexpr std::string_view name = "tree";
        static constexpr user::Permissions required_perms = user::Permissions::none;

        void parse_fields(std::span<const Field> fields);
        void execute();
    };
}
