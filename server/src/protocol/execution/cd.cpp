#include "tds/protocol/execution/cd.hpp"

#include "tds/protocol/protocol_error.hpp"
#include <filesystem>

#include <fmt/core.h>

namespace fs = std::filesystem;

namespace tds::protocol::execution {
    void Cd::parse_fields(std::span<const Field> fields) {
        for(auto&& field : fields) {
            if(field.get_name() == "path") {
                parse_path(field);
            }
        }

        if(!m_path.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "path field is missing"};
        }
    }

    void Cd::execute() {
        m_response_builder.set_code(ProtocolCode::ok);
        m_response_builder.set_command_name("cd");

        m_client_context->set_current_path(std::move(*m_path));
        m_response_builder.add_line(
            m_server_context->get_relative_path_of(m_client_context->get_current_path()).native());
    }

    void Cd::parse_path(const Field& path_field) {
        if(m_path.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "path was already set"};
        }

        const auto path_value = path_field.get_string();
        if(!path_value.has_value()) {
            throw ProtocolError{ProtocolCode::invalid_field_value,
                                "path field has invalid value type (should be string)"};
        }

        fs::path requested_path = *path_value;
        fs::path final_path;

        if(requested_path.is_absolute()) {
            final_path = fs::path{m_server_context->get_root_path()} += requested_path;
        } else {
            fs::path suffix = ".//";
            suffix += fs::path{*path_value}.lexically_normal();
            final_path = (m_client_context->get_current_path() / suffix).lexically_normal();

            if(!final_path.native().starts_with(m_server_context->get_root_path().native())) {
                final_path = m_server_context->get_root_path();
            }
        }

        if(!fs::exists(final_path)) {
            throw ProtocolError{ProtocolCode::not_found, fmt::format("directory '{}' does not exist", *path_value)};
        }

        m_path.emplace(std::move(final_path));
    }
}
