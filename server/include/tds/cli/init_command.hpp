#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class init_command {
    public:
        init_command() = default;
        init_command(const init_command&) = delete;
        init_command& operator=(const init_command&) = delete;

        static std::string_view name();

        void execute(std::span<const std::string_view> args);
    };
}
