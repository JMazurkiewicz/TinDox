#pragma once

#include <concepts>
#include <span>
#include <string_view>

// clang-format off
namespace tds::cli {
    template<typename T>
    concept command =
        std::default_initializable<T> &&
        requires(T& com, std::span<const std::string_view> args) {
            { T::name() } -> std::convertible_to<std::string_view>;
            { com.execute(args) } -> std::same_as<void>;
        };
}
