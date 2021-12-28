#include "tds/protocol/protocol_interpreter.hpp"

#include <algorithm>
#include <charconv>
#include <ranges>
#include <stdexcept>

#include <spdlog/spdlog.h>

using namespace std::string_view_literals;

namespace tds::protocol {
    ProtocolInterpreter::ProtocolInterpreter()
        : m_last_line_complete{true}
        , m_last_checked_line{0} { }

    void ProtocolInterpreter::add_bytes(std::span<const char> input) {
        const auto end = input.end();

        for(auto it = input.begin(); it != end; ++it) {
            auto line_end = std::ranges::find(it, end, line_separator);
            std::string now{it, line_end};
            const bool out_of_data = (line_end == end);

            if(m_last_line_complete && now.empty()) {
                add_response();
            }

            if(m_last_line_complete) {
                m_lines.push_back(std::string{});
            }
            m_lines.back() += now;

            if(now.empty()) {
                add_response();
                m_last_line_complete = true;
            } else {
                m_last_line_complete = out_of_data;
                if(out_of_data) {
                    it = line_end;
                }
            }
        }
    }

    void ProtocolInterpreter::add_response() {
        // TODO: throws are temporary solution (project needs ProtocolError class)
        if(m_requests.empty()) {
            m_requests.clear();
            return;
        }

        Request request{std::move(m_lines.front())};
        for(const std::string& line : m_lines | std::views::drop(1)) {
            const std::size_t first_char_of_name = line.find_first_not_of(allowed_whitespaces);
            if(first_char_of_name == std::string::npos) {
                throw std::runtime_error{"#1"};
            }

            const std::size_t first_char_of_ws = line.find_first_of(allowed_whitespaces, first_char_of_name);
            if(first_char_of_ws == std::string::npos) {
                throw std::runtime_error{"#2"};
            }

            std::string field_name{line, first_char_of_name, first_char_of_ws - first_char_of_name};
            if(std::ranges::any_of(field_name, [](char c) { return c < 'a' || 'z' > c; })) {
                throw std::runtime_error{"#3"};
            }

            const std::size_t separator_position = line.find_first_not_of(allowed_whitespaces, first_char_of_ws);
            if(separator_position == std::string::npos || line[separator_position] != field_separator) {
                throw std::runtime_error{"#4"};
            }

            const std::size_t first_char_of_value = line.find_first_not_of(allowed_whitespaces, separator_position + 1);
            if(first_char_of_value == std::string::npos) {
                throw std::runtime_error{"#5"};
            }

            const std::size_t last_char_of_value = line.find_last_not_of(allowed_whitespaces);
            std::string field_value{line, first_char_of_value, last_char_of_value - first_char_of_value};
            if(field_value.empty()) {
                throw std::runtime_error{"#6"};
            }

            std::optional<Field> field;
            if(std::ranges::equal(field_value, "true"sv)) {
                field.emplace(std::move(field_name), true);
            } else if(std::ranges::equal(field_value, "false"sv)) {
                field.emplace(std::move(field_name), false);
            } else if(std::ranges::all_of(field_value, [](char c) { return '0' <= c && c <= '9'; })) {
                std::uintmax_t value = 0;
                std::from_chars(field_value.data(), field_value.data() + field_value.size(),
                                value); // TODO handle errors
                field.emplace(std::move(field_name), value);
            } else {
                field.emplace(std::move(field_name), std::move(field_value));
            }

            request.add_field(std::move(*field));
        }

        m_requests.push_back(std::move(request));
    }

    bool ProtocolInterpreter::has_available_requests() {
        return !m_requests.empty();
    }

    std::vector<Request> ProtocolInterpreter::get_requests() {
        return std::move(m_requests);
    }
}
