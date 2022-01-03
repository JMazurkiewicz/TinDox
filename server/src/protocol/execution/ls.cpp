#include "tds/protocol/execution/ls.hpp"

#include <chrono>

#include <fmt/chrono.h>
#include <fmt/core.h>

namespace chrono = std::chrono;
namespace fs = std::filesystem;

namespace tds::protocol::execution {
    Ls::Ls()
        : m_return_size{false}
        , m_return_mod{false} { }

    void Ls::parse_fields(std::span<const Field> fields) {
        for(auto&& field : fields) {
            if(field.get_name() == "path") {
                parse_path(field);
            } else if(field.get_name() == "size") {
                parse_size(field);
            } else if(field.get_name() == "mod") {
                parse_mod(field);
            }
        }
    }

    void Ls::execute() {
        if(!m_path.has_value()) {
            m_path.emplace(m_client_context->get_current_path());
        }

        for(auto&& entry : fs::directory_iterator{*m_path}) {
            std::string line;
            line += '"' + entry.path().filename().native() + '"';

            if(m_return_size) {
                std::error_code errc;
                const auto size = entry.file_size(errc);

                line += ' ';
                if(errc.value() == 0) {
                    line += std::to_string(size);
                } else {
                    line += '-';
                }
            }

            if(m_return_mod) {
                std::error_code errc;
                const std::time_t lwt =
                    chrono::system_clock::to_time_t(chrono::file_clock::to_sys(entry.last_write_time(errc)));

                line += ' ';
                if(errc.value() == 0) {
                    line += fmt::format("\"{:%d.%m.%Y %T}\"", fmt::localtime(lwt));
                } else {
                    line += '-';
                }
            }

            m_response_builder.add_line(line);
        }
    }

    void Ls::parse_path(const Field& path_field) {
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

    void Ls::parse_size(const Field& size_field) {
        if(auto size_value = size_field.get_boolean()) {
            m_return_size = *size_value;
        } else {
            throw ProtocolError{ProtocolCode::invalid_field_value,
                                "size field has invalid value type (should be boolean)"};
        }
    }

    void Ls::parse_mod(const Field& mod_field) {
        if(auto mod_value = mod_field.get_boolean()) {
            m_return_mod = *mod_value;
        } else {
            throw ProtocolError{ProtocolCode::invalid_field_value,
                                "mod field has invalid value type (should be boolean)"};
        }
    }
}
