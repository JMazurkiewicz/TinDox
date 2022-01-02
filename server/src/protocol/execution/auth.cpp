#include "tds/protocol/execution/auth.hpp"

#include "tds/protocol/auth_token.hpp"
#include "tds/protocol/protocol_code.hpp"
#include "tds/protocol/protocol_error.hpp"

namespace tds::protocol::execution {
    void Auth::parse_fields(std::span<const Field> fields) {
        for(auto&& field : fields) {
            if(field.get_name() == "login") {
                parse_username(field);
            } else if(field.get_name() == "passwd") {
                parse_password(field);
            }
        }

        if(!m_username.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "login field is missing"};
        }

        if(!m_password.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "passwd field is missing"};
        }
    }

    void Auth::execute() {
        m_response_builder.set_code(ProtocolCode::ok);
        m_response_builder.set_command_name(name);
        m_client_context->set_new_token(m_server_context->authorize_user(*m_username, *m_password));
    }

    void Auth::parse_username(const Field& login_field) {
        if(m_username.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "login was already set"};
        } else if(auto login_value = login_field.get_string()) {
            m_username.emplace(*login_value);
        } else {
            throw ProtocolError{ProtocolCode::invalid_field_value, "login field has invalid value type"};
        }
    }

    void Auth::parse_password(const Field& passwd_field) {
        if(m_password.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "passwd was already set"};
        } else if(auto passwd_value = passwd_field.get_string()) {
            m_password.emplace(*passwd_value);
        } else {
            throw ProtocolError{ProtocolCode::invalid_field_value, "passwd field has invalid value type"};
        }
    }
}
