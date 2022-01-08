#pragma once

#include <span>
#include <string_view>

namespace tds::cli {
    class VersionCommand {
    public:
        static constexpr std::string_view name = "version";

        void parse_arguments(std::span<const std::string_view> args);
        void execute();

    private:
        bool m_empty_args = true;
    };
}
