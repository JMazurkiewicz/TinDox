#pragma once

#include "tds/protocol/execution/transport_command.hpp"

namespace tds::protocol::execution {
    class Mv : public TransportCommand {
    public:
        static constexpr std::string_view name = "mv";
        static constexpr user::Permissions required_perms = user::Permissions::move;
        static constexpr bool requires_authorization = true;

        void execute();
    };
}
