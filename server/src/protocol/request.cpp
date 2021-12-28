#include "tds/protocol/request.hpp"

namespace tds::protocol {
    Request::Request(std::string name)
        : m_name{std::move(name)} { }

    std::string_view Request::get_name() const noexcept {
        return m_name;
    }

    std::span<const Field> Request::get_fields() const noexcept {
        return m_fields;
    }

    void Request::add_field(Field new_filed) {
        m_fields.emplace_back(std::move(new_filed));
    }
}
