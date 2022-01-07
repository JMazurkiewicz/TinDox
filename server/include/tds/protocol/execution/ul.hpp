#pragma once

#include "tds/protocol/execution/command_base.hpp"

namespace tds::protocol::execution {
    class Ul : public CommandBase {
    public:
        static constexpr std::string_view name = "ul";
        static constexpr user::Permissions required_perms = user::Permissions::upload;
        static constexpr bool requires_authorization = true;

        void parse_fields(std::span<const Field> fields);
        void execute();

    private:
        void parse_name(const Field& name_field);
        void parse_size(const Field& size_field);
        void parse_retry(const Field& retry_field);

        void retry();

        std::optional<std::filesystem::path> m_name;
        std::optional<std::uintmax_t> m_size;
        bool m_retry : 1;
    };
}
