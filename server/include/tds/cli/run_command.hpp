#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class run_command {
    public:
        run_command() = default;
        run_command(const run_command&) = delete;
        run_command& operator=(const run_command&) = delete;

        static std::string_view name();

        void execute(std::span<const std::string_view> args);
    };
}
