#include "tds/protocol/field.hpp"

#include "tds/protocol/field_value.hpp"

#include <algorithm>
#include <charconv>
#include <stdexcept> // TODO -- Field should use protocol error
#include <fmt/format.h> // TODO -- remove

using namespace std::string_view_literals;

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

    Field field_from_string(std::string_view str) {
        static constexpr std::string_view allowed_whitespaces = " \t";
        static constexpr char field_separator = ':';

        const std::size_t first_char_of_name = str.find_first_not_of(allowed_whitespaces);
        if(first_char_of_name == std::string::npos) {
            // Expected str starting with field name
            throw std::runtime_error{fmt::format("FCON: {}", first_char_of_name)};
        }

        const auto first_char_after_name_iterator =
            std::ranges::find_if_not(str.begin() + first_char_of_name, str.end(),
                                     [](char c) { return 'a' <= c && c <= 'z'; });
        const std::size_t first_char_after_name = first_char_after_name_iterator - str.begin();
        if(first_char_after_name == std::string::npos) {
            // expected
            throw std::runtime_error{fmt::format("FCAN: {}", first_char_after_name)};
        }

        std::string field_name{str, first_char_of_name, first_char_after_name - first_char_of_name};
        if(std::ranges::any_of(field_name, [](char c) { return c < 'a' || 'z' < c; })) {
            throw std::runtime_error{fmt::format("FN: '{}'", field_name)};
        }

        const std::size_t separator_position = str.find_first_not_of(allowed_whitespaces, first_char_after_name);
        if(separator_position == std::string::npos || str[separator_position] != field_separator) {
            throw std::runtime_error{fmt::format("SP: {}", separator_position)};
        }

        const std::size_t first_char_of_value = str.find_first_not_of(allowed_whitespaces, separator_position + 1);
        if(first_char_of_value == std::string::npos) {
            throw std::runtime_error{fmt::format("FCOV: {}", first_char_of_value)};
        }

        const std::size_t last_char_of_value = str.find_last_not_of(allowed_whitespaces);
        if(last_char_of_value == std::string::npos) {
            throw std::runtime_error{"should have checked lol"}; // TODO XD
        }

        std::string field_value{str, first_char_of_value, last_char_of_value - first_char_of_value + 1};
        if(field_value.empty()) {
            throw std::runtime_error{"#6"};
        }

        if(std::ranges::equal(field_value, "true"sv)) {
            return Field{std::move(field_name), true};
        } else if(std::ranges::equal(field_value, "false"sv)) {
            return Field{std::move(field_name), false};
        } else if(std::ranges::all_of(field_value, [](char c) { return '0' <= c && c <= '9'; })) {
            std::uintmax_t value = 0;
            std::from_chars(field_value.data(), field_value.data() + field_value.size(),
                            value); // TODO handle errors
            return Field{std::move(field_name), value};
        } else {
            return Field{std::move(field_name), std::move(field_value)};
        }
    }
}
