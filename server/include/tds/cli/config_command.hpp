#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class ConfigCommand {
    public:
        ConfigCommand() = default;
        ConfigCommand(const ConfigCommand&) = delete;
        ConfigCommand& operator=(const ConfigCommand&) = delete;

        static std::string_view name();

        void execute([[maybe_unused]] std::span<const std::string_view>);
    };
}
