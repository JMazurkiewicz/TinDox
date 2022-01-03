#pragma once

#include "tds/protocol/execution/transport_command.hpp"

namespace tds::protocol::execution {
    class Cp : public TransportCommand {
    public:
        static constexpr std::string_view name = "cp";
        static constexpr user::Permissions required_perms = user::Permissions::copy;

        void execute();
    };
}
