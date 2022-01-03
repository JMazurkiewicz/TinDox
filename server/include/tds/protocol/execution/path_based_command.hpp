#pragma once

#include "tds/protocol/execution/command_base.hpp"

namespace tds::protocol::execution {
    class PathBasedCommand : public CommandBase {
    public:
        static constexpr bool requires_authorization = true;

        void parse_fields(std::span<const Field> fields);

    protected:
        bool has_path() const noexcept;
        std::filesystem::path& get_path();
        const std::filesystem::path& get_path() const;
        void set_path(std::filesystem::path path);

    private:
        void parse_path(const Field& path_field);
        std::optional<std::filesystem::path> m_path;
    };
}
