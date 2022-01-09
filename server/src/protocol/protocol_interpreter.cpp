#include "tds/protocol/protocol_interpreter.hpp"

#include "tds/protocol/protocol_error.hpp"

#include <algorithm>
#include <ranges>
#include <stdexcept>

#include <fmt/core.h>

namespace tds::protocol {
    ProtocolInterpreter::ProtocolInterpreter()
        : m_last_line_complete{true} { }

    void ProtocolInterpreter::restart() {
        m_lines.clear();
        m_request.reset();
        m_last_line_complete = true;
    }

    void ProtocolInterpreter::commit_bytes(std::span<const char>& bytes) {
        while(!bytes.empty() && !has_available_request()) {
            auto line_end = std::ranges::find(bytes, line_separator);
            if(line_end != bytes.end()) {
                ++line_end;
            }

            const std::span<const char> line{bytes.begin(), line_end};
            bytes = {line_end, bytes.end()};
            commit_line(line);
        }
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
            if(m_lines.size() > 1) {
                m_lines.pop_back();
                commit_request();
            }
            m_lines.clear();
        }
    }

    void ProtocolInterpreter::check_for_protocol_errors() {
        const auto line_length = m_lines.back().size();
        const auto line_count = m_lines.size();

        const bool too_long_line = (line_length > max_line_length);
        const bool too_many_lines = (line_count > max_line_count);
        if(too_long_line || too_many_lines) {
            restart();
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
        restart();
        return final_request;
    }
}
