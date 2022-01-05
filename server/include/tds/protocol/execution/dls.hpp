#pragma once

#include "tds/protocol/execution/command_base.hpp"

namespace tds::protocol::execution {
    class Dls : public CommandBase {
    public:
        static constexpr std::string_view name = "dls";
        static constexpr user::Permissions required_perms = user::Permissions::download;
        static constexpr bool requires_authorization = true;

        Dls();
        void parse_fields(std::span<const Field> fields);
        void execute();

    private:
        void parse_offset(const Field& offset_field);
        std::uintmax_t m_offset;
    };
}
