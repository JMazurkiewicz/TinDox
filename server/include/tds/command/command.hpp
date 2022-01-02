#pragma once

#include <concepts>
#include <span>
#include <string_view>

namespace tds::command {
    // clang-format off
    template<typename T>
    concept Command =
        std::default_initializable<T> &&
        requires(T& command, std::span<const std::string_view> args) {
            { T::name } -> std::convertible_to<std::string_view>;
            requires std::bool_constant<!std::string_view{T::name}.empty()>::value;
            { command.execute() } -> std::same_as<void>;
        };
}
