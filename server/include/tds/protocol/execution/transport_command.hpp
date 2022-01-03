#pragma once

#include "tds/protocol/execution/path_based_command.hpp"

namespace tds::protocol::execution {
    class TransportCommand : public PathBasedCommand {
    public:
        void parse_fields(std::span<const Field> fields);

    protected:
        const std::filesystem::path& get_name() const;

    private:
        void parse_name(const Field& name_field);
        std::optional<std::filesystem::path> m_name;
    };
};
