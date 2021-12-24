#include "tds/protocol/response.hpp"

#include <algorithm>

#include <fmt/core.h>

namespace tds::protocol {
    Response::Response(Code code, std::string_view name)
        : m_data{make_response_header(code, name)}
        , m_finished{false}
        , m_offset{0} { }

    void Response::add_line(std::string_view line) {
        if(!m_finished) {
            m_data += line;
            m_data += '\n';
        }
    }

    void Response::finish() {
        m_data += '\n';
        m_finished = true;
    }

    std::size_t Response::get_offset() const noexcept {
        return m_finished ? m_offset : 0;
    }

    std::size_t Response::get_remaining_byte_count() const noexcept {
        return m_data.size() - get_offset();
    }

    void Response::add_offset(std::size_t offset) {
        if(m_finished) {
            m_offset += std::ranges::min(offset, get_remaining_byte_count());
        }
    }

    bool Response::is_fully_sent() const noexcept {
        return get_remaining_byte_count() == 0;
    }

    const char* Response::get_current_data() const noexcept {
        return m_data.data() + m_offset;
    }

    std::string Response::make_response_header(Code code, std::string_view name) {
        return fmt::format("{} {}\n", static_cast<int>(code), name);
    }
}
