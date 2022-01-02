#pragma once

#include "tds/protocol/execution/command_base.hpp"

namespace tds::protocol::execution {
    class Pwd : public CommandBase {
    public:
        static constexpr std::string_view name = "pwd";

        void parse_fields(std::span<const Field> fields);
        void execute();
    };
}
