#pragma once

#include "tds/command/command.hpp"
#include "tds/protocol/client_context.hpp"
#include "tds/protocol/execution/command_base.hpp"
#include "tds/protocol/field.hpp"
#include "tds/protocol/response.hpp"
#include "tds/protocol/server_context.hpp"
#include "tds/user/permissions.hpp"

#include <span>
#include <utility>

namespace tds::protocol::execution {
    // clang-format off
    template<typename T>
    concept Command =
        tds::command::Command<T> &&
        std::derived_from<T, CommandBase> &&
        requires(T& command) {
            { T::required_perms } -> std::convertible_to<user::Permissions>;
            { T::requires_authorization } -> std::convertible_to<bool>;

            { command.set_server_context(std::declval<const ServerContext&>()) } -> std::same_as<void>;
            { command.set_client_context(std::declval<ClientContext&>()) } -> std::same_as<void>;

            { command.parse_fields(std::declval<std::span<const Field>>()) } -> std::same_as<void>;
            { command.get_response() } -> std::same_as<Response>;
        };
}
