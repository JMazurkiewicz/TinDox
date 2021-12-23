#include "tds/protocol/field.hpp"

#include "tds/protocol/field_value.hpp"

namespace tds::protocol {
    Field::Field(std::string name, FieldValue value)
        : m_name{std::move(name)}
        , m_value{std::move(value)} { }

    std::string_view Field::get_name() const noexcept {
        return m_name;
    }

    FieldValueType Field::get_value_type() const noexcept {
        return static_cast<FieldValueType>(m_value.index());
    }

    const FieldValue& Field::get_value() const noexcept {
        return m_value;
    }
}
