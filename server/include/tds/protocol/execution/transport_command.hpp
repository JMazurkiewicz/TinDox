#pragma once

#include "tds/protocol/execution/command.hpp"

namespace tds::protocol::execution {
    class TransportCommand : public CommandBase {
    public:
        void parse_fields(std::span<const Field> fields);

    protected:
        const std::filesystem::path& get_name() const;
        const std::filesystem::path& get_path() const;

    private:
        void parse_name(const Field& name_field);
        void parse_path(const Field& path_field);

        std::optional<std::filesystem::path> m_name;
        std::optional<std::filesystem::path> m_path;
    };
};
