#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class UserCommand {
    public:
        UserCommand() = default;
        UserCommand(const UserCommand&) = delete;
        UserCommand& operator=(const UserCommand&) = delete;

        static std::string_view name();

        void execute(std::span<const std::string_view> args);
    };
}
