#pragma once

#include "tds/protocol/execution/command_base.hpp"

namespace tds::protocol::execution {
    class Ls : public CommandBase {
    public:
        static constexpr std::string_view name = "ls";

        void parse_fields(std::span<const Field> fields);
        void execute();
    };
}
