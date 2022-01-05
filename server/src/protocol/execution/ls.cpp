#include "tds/protocol/execution/ls.hpp"

#include "tds/protocol/execution/path_based_command.hpp"
#include "tds/protocol/server_context.hpp"

#include <chrono>

#include <fmt/chrono.h>
#include <fmt/core.h>

namespace tds::protocol::execution {
    Ls::Ls()
        : m_return_size{false}
        , m_return_mod{false} { }

    void Ls::parse_fields(std::span<const Field> fields) {
        PathBasedCommand::parse_fields(fields);
        for(auto&& field : fields) {
            if(field.get_name() == "size") {
                parse_size(field);
            } else if(field.get_name() == "mod") {
                parse_mod(field);
            }
        }
    }

    void Ls::execute() {
        if(!has_path()) {
            set_path(m_client_context->get_current_path());
        }

        for(auto&& entry : std::filesystem::directory_iterator{get_path()}) {
            if(m_server_context->is_forbidden(entry.path())) {
                continue;
            }

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
                    std::chrono::system_clock::to_time_t(std::chrono::file_clock::to_sys(entry.last_write_time(errc)));

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
