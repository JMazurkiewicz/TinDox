#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class RunCommand {
    public:
        RunCommand() = default;
        RunCommand(const RunCommand&) = delete;
        RunCommand& operator=(const RunCommand&) = delete;

        static std::string_view name();

        void execute(std::span<const std::string_view> args);
    };
}
