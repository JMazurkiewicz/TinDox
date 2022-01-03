#pragma once

#include "tds/protocol/execution/path_based_command.hpp"

namespace tds::protocol::execution {
    class Ls : public PathBasedCommand {
    public:
        static constexpr std::string_view name = "ls";
        static constexpr user::Permissions required_perms = user::Permissions::none;

        Ls();
        void parse_fields(std::span<const Field> fields);
        void execute();

    private:
        void parse_size(const Field& size_field);
        void parse_mod(const Field& mod_field);

        bool m_return_size : 1;
        bool m_return_mod : 1;
    };
}
