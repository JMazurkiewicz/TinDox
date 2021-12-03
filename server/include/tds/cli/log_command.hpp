#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class log_command {
    public:
        log_command() = default;
        log_command(const log_command&) = delete;
        log_command& operator=(const log_command&) = delete;

        static std::string_view name();

        void execute(std::span<const std::string_view> args);
    };
}
