#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class InitCommand {
    public:
        InitCommand() = default;
        InitCommand(const InitCommand&) = delete;
        InitCommand& operator=(const InitCommand&) = delete;

        static std::string_view name();

        void execute(std::span<const std::string_view> args);
    };
}
