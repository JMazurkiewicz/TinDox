#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class HelpCommand {
    public:
        HelpCommand() = default;
        HelpCommand(const HelpCommand&) = delete;
        HelpCommand& operator=(const HelpCommand&) = delete;

        static std::string_view name();

        void execute([[maybe_unused]] std::span<const std::string_view>);
    };
}
