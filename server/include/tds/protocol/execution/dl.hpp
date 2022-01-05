#pragma once

#include "tds/protocol/execution/command_base.hpp"

namespace tds::protocol::execution {
    class Dl : public CommandBase {
    public:
        static constexpr std::string_view name = "dl";
        static constexpr user::Permissions required_perms = user::Permissions::download;
        static constexpr bool requires_authorization = true;

        void parse_fields(std::span<const Field> fields);
        void execute();

    private:
        void parse_name(const Field& name_field);
        std::optional<std::filesystem::path> m_name;
    };
}
