#include "tds/protocol/execution/ul.hpp"

#include "tds/protocol/protocol_error.hpp"
#include "tds/protocol/protocol_mode.hpp"

#include <algorithm>

#include <toml++/toml.h>

namespace fs = std::filesystem;

namespace tds::protocol::execution {
    void Ul::parse_fields(std::span<const Field> fields) {
        for(auto&& field : fields) {
            if(field.get_name() == "name") {
                parse_name(field);
            } else if(field.get_name() == "size") {
                parse_size(field);
            } else if(field.get_name() == "retry") {
                parse_retry(field);
            }
        }

        if(!m_name.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "name field is missing"};
        } else if(!m_size.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "size field is missing"};
        }
    }

    void Ul::execute() {
        auto upload_token = m_retry ? retry_upload() : start_new_upload();
        upload_token->set_temporary_filename_stem(m_client_context->get_auth_token().get_username());
        m_response_builder.add_line(std::to_string(upload_token->get_file_offset()));

        m_client_context->set_upload_token(std::move(upload_token));
        m_client_context->set_mode(ProtocolMode::upload);
    }

    void Ul::parse_name(const Field& name_field) {
        if(m_name.has_value()) {
            throw ProtocolError{ProtocolCode::bad_field, "name was already set"};
        }

        const auto name_value = name_field.get_string();
        if(!name_value.has_value()) {
            throw ProtocolError{ProtocolCode::invalid_field_value,
                                "name field has invalid value type (should be string)"};
        }

        fs::path name_as_path = *name_value;
        if(name_as_path != name_as_path.filename()) {
            throw ProtocolError{ProtocolCode::invalid_field_value, "name must be just a name"};
        } else {
            m_name.emplace(std::move(name_as_path));
        }
    }

    void Ul::parse_size(const Field& size_field) {
        const auto size = size_field.get_integer();
        if(!size.has_value()) {
            throw ProtocolError{ProtocolCode::invalid_field_value, "size has invalid value type (should be integer)"};
        } else {
            m_size = *size;
        }
    }

    void Ul::parse_retry(const Field& retry_field) {
        if(auto retry_value = retry_field.get_boolean()) {
            m_retry = *retry_value;
        } else {
            throw ProtocolError{ProtocolCode::invalid_field_value,
                                "retry field has invalid value type (should be boolean)"};
        }
    }

    std::shared_ptr<UploadToken> Ul::start_new_upload() {
        const fs::path full_path = m_client_context->get_current_path() / *m_name;
        auto token = m_server_context->upload_file(full_path, *m_size);
        return token;
    }

    std::shared_ptr<UploadToken> Ul::retry_upload() {
        const fs::path backup_path =
            m_server_context->get_backup_file_path(m_client_context->get_auth_token().get_username());
        toml::table table;

        try {
            table = toml::parse_file(backup_path.native());
        } catch(const toml::parse_error&) {
            throw ProtocolError{ProtocolCode::no_upload_to_resume};
        }

        auto path = table["path"].as_string();
        auto size = table["size"].as_integer();
        auto offset = table["offset"].as_integer();
        fs::remove(backup_path);

        if(!path || !size || !offset) {
            throw ProtocolError{ProtocolCode::no_upload_to_resume};
        } else {
            auto token = m_server_context->upload_file(path->get(), size->get());
            token->add_file_offset(offset->get());
            return token;
        }
    }
}
