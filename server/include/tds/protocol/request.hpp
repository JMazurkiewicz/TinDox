#pragma once

#include "tds/protocol/field.hpp"

#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace tds::protocol {
    class Request {
    public:
        Request(const Request&) = delete;
        Request& operator=(const Request&) = delete;
        Request(Request&&) = default;
        Request& operator=(Request&&) = default;

        std::string_view get_name() const noexcept;
        void set_name(std::string name);

        std::span<const Field> get_fields() const noexcept;
        void add_field(Field new_filed);

    private:
        std::string m_name;
        std::vector<Field> m_fields;
    };
}
