#pragma once

#include "tds/protocol/field_value.hpp"

#include <string_view>

namespace tds::protocol {
    class Field {
    public:
        explicit Field(std::string name, FieldValue value);

        Field(const Field&) = delete;
        Field& operator=(const Field&) = delete;
        Field(Field&&) = default;
        Field& operator=(Field&&) = default;

        std::string_view get_name() const noexcept;

        FieldValueType get_value_type() const noexcept;
        const FieldValue& get_value() const noexcept;

        template<typename F>
        void visit_value(F&& visitor) const;

    private:
        std::string m_name;
        FieldValue m_value;
    };

    template<typename F>
    void Field::visit_value(F&& visitor) const {
        std::visit(std::forward<F>(visitor), m_value);
    }
}
