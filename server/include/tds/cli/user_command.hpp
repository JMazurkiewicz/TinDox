#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class user_command {
    public:
        user_command() = default;
        user_command(const user_command&) = delete;
        user_command& operator=(const user_command&) = delete;

        static std::string_view name();

        void execute(std::span<const std::string_view> args);
    };
}
