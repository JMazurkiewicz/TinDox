#pragma once

#include "tds/protocol/execution/command_base.hpp"
#include "tds/protocol/field.hpp"

#include <span>

namespace tds::protocol::execution {
    class Cd : public CommandBase {
    public:
        static constexpr std::string_view name = "cd";

        void parse_fields(std::span<const Field> fields);
        void execute();

    private:
        std::filesystem::path m_next_path;
    };
}
