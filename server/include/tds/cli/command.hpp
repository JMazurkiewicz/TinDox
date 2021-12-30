#pragma once

#include "tds/command/command.hpp"

namespace tds::cli {
    // clang-format off
    template<typename T>
    concept Command =
        command::Command<T> &&
        requires(T& command, std::span<const std::string_view> args) {
            { command.parse_arguments(args) } -> std::same_as<void>;
        };
}
