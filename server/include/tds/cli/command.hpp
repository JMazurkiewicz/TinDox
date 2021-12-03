#pragma once

#include <concepts>
#include <span>
#include <string_view>

namespace tds::cli {
    struct CommandBase {
        CommandBase() = default;
        CommandBase(const CommandBase&) = delete;
        CommandBase& operator=(const CommandBase&) = delete;
    };

    // clang-format off
    template<typename T>
    concept Command =
        std::derived_from<T, CommandBase> &&
        std::default_initializable<T> &&
        !std::copyable<T> &&
        requires(T& com, std::span<const std::string_view> args) {
            { com.do_execute(args) } -> std::same_as<int>;
            { T::name } -> std::convertible_to<std::string_view>;
            requires std::bool_constant<!std::string_view{T::name}.empty()>::value;
        };
}
