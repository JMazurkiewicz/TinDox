#pragma once

#include "tds/protocol/execution/command_base.hpp"

namespace tds::protocol::execution {
    class Ls : public CommandBase {
    public:
        static constexpr std::string_view name = "ls";
        static constexpr user::Permissions required_perms = user::Permissions::none;
        static constexpr bool requires_authorization = true;

        Ls();
        void parse_fields(std::span<const Field> fields);
        void execute();

    private:
        void parse_path(const Field& path_field);
        void parse_size(const Field& size_field);
        void parse_mod(const Field& mod_field);

        std::optional<std::filesystem::path> m_path;
        bool m_return_size : 1;
        bool m_return_mod : 1;
    };
}
