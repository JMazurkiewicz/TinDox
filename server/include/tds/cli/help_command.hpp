#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class HelpCommand {
    public:
        static constexpr std::string_view name = "help";

        void parse_arguments(std::span<const std::string_view> args);
        void execute();

    private:
        bool m_empty_args = true;
    };
}
