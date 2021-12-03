#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class LogCommand {
    public:
        LogCommand() = default;
        LogCommand(const LogCommand&) = delete;
        LogCommand& operator=(const LogCommand&) = delete;

        static std::string_view name();

        void execute(std::span<const std::string_view> args);
    };
}
