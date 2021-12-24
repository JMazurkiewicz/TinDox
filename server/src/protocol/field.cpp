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

    std::optional<bool> Field::get_boolean() const noexcept {
        if(auto* val = std::get_if<bool>(&get_value())) {
            return *val;
        } else {
            return std::nullopt;
        }
    }

    std::optional<std::uintmax_t> Field::get_integer() const noexcept {
        if(auto* val = std::get_if<std::uintmax_t>(&get_value())) {
            return *val;
        } else {
            return std::nullopt;
        }
    }

    std::optional<std::string_view> Field::get_string() const noexcept {
        if(auto* val = std::get_if<std::string>(&get_value())) {
            return *val;
        } else {
            return std::nullopt;
        }
    }
}
