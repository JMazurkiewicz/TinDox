#include "tds/protocol/execution/path_based_command.hpp"

#include <algorithm>

#include <fmt/core.h>

namespace fs = std::filesystem;
using namespace std::string_view_literals;

namespace tds::protocol::execution {
    void PathBasedCommand::parse_fields(std::span<const Field> fields) {
        if(const auto it = std::ranges::find(fields, "path"sv, &Field::get_name); it != fields.end()) {
            parse_path(*it);
        }
    }

    bool PathBasedCommand::has_path() const noexcept {
        return m_path.has_value();
    }

    std::filesystem::path& PathBasedCommand::get_path() {
        return m_path.value();
    }

    const std::filesystem::path& PathBasedCommand::get_path() const {
        return m_path.value();
    }

    void PathBasedCommand::set_path(std::filesystem::path path) {
        m_path.emplace(std::move(path));
    }

    void PathBasedCommand::parse_path(const Field& path_field) {
        if(m_path.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "path was already set"};
        }

        const auto path_value = path_field.get_string();
        if(!path_value.has_value()) {
            throw ProtocolError{ProtocolCode::invalid_field_value,
                                "path field has invalid value type (should be string)"};
        }

        const fs::path requested_path = *path_value;
        fs::path final_path;

        if(requested_path.is_absolute()) {
            final_path = fs::path{m_server_context->get_root_path()} += requested_path;
        } else {
            final_path = (m_client_context->get_current_path() / requested_path).lexically_normal();
            if(!final_path.native().starts_with(m_server_context->get_root_path().native())) {
                final_path = m_server_context->get_root_path();
            }
        }

        if(!fs::exists(final_path)) {
            throw ProtocolError{ProtocolCode::not_found, fmt::format("directory '{}' does not exist", *path_value)};
        }

        set_path(std::move(final_path));
    }
}
