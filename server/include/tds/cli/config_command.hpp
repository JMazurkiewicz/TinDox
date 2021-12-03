#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class config_command {
    public:
        config_command() = default;
        config_command(const config_command&) = delete;
        config_command& operator=(const config_command&) = delete;

        static std::string_view name();

        void execute([[maybe_unused]] std::span<const std::string_view>);
    };
}
