#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class help_command {
    public:
        help_command() = default;
        help_command(const help_command&) = delete;
        help_command& operator=(const help_command&) = delete;

        static std::string_view name();

        void execute([[maybe_unused]] std::span<const std::string_view>);
    };
}
