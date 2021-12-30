#include "tds/protocol/protocol_interpreter.hpp"

#include "tds/protocol/protocol_error.hpp"

#include <algorithm>
#include <ranges>
#include <stdexcept>

#include <fmt/core.h>

namespace tds::protocol {
    ProtocolInterpreter::ProtocolInterpreter()
        : m_last_line_complete{true} { }

    std::span<const char> ProtocolInterpreter::commit_bytes(std::span<const char> bytes) {
        auto cur = bytes.begin();
        const auto end = bytes.end();

        while(cur != end && !has_available_request()) {
            auto line_end = std::ranges::find(cur, end, line_separator);
            if(line_end != end) {
                ++line_end;
            }

            commit_line(std::span{cur, line_end});
            cur = line_end;
        }

        return bytes.last(end - cur);
    }

    void ProtocolInterpreter::commit_line(std::span<const char> line) {
        if(m_last_line_complete) {
            m_lines.emplace_back(line.begin(), line.end());
        } else {
            m_lines.back().append(line.begin(), line.end());
        }
        check_for_protocol_errors();

        m_last_line_complete = (m_lines.back().back() == '\n');
        if(m_last_line_complete) {
            m_lines.back().pop_back();
        }

        if(m_lines.back().empty()) {
            m_lines.pop_back();
            commit_request();
            m_lines.clear();
        }
    }

    void ProtocolInterpreter::check_for_protocol_errors() {
        const bool too_long_line = (m_lines.back().size() > max_line_length);
        const bool too_many_lines = (m_lines.size() > max_line_count);
        if(too_long_line || too_many_lines) {
            m_last_line_complete = true;
        }

        if(too_long_line) {
            throw ProtocolError{ProtocolCode::too_long_line,
                                fmt::format("Too long line ({} bytes)", m_lines.back().size())};
        } else if(too_many_lines) {
            throw ProtocolError{ProtocolCode::too_many_fields, fmt::format("Too many fields ({})", m_lines.size())};
        }
    }

    void ProtocolInterpreter::commit_request() {
        if(m_request.has_value()) {
            throw std::runtime_error{
                fmt::format("ProtocolInterpreter: last request '{}' was not properly handled", m_request->get_name())};
        }

        Request request{std::move(m_lines.front())};
        for(std::string_view line : m_lines | std::views::drop(1)) {
            request.add_field(field_from_string(line));
        }

        m_request.emplace(std::move(request));
    }

    bool ProtocolInterpreter::has_available_request() const noexcept {
        return m_request.has_value();
    }

    Request ProtocolInterpreter::get_request() {
        Request final_request = std::move(m_request).value();
        m_request.reset();
        return final_request;
    }
}
