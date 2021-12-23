#include "tds/protocol/request.hpp"

namespace tds::protocol {
    std::string_view Request::get_name() const noexcept {
        return m_name;
    }

    void Request::set_name(std::string name) {
        m_name = std::move(name);
    }

    std::span<const Field> Request::get_fields() const noexcept {
        return m_fields;
    }

    void Request::add_field(Field new_filed) {
        m_fields.emplace_back(std::move(new_filed));
    }
}
