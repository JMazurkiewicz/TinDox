#include "tds/protocol/response.hpp"

#include <algorithm>

namespace tds::protocol {
    Response::Response(std::string data)
        : m_data{std::move(data)}
        , m_offset{0} { }

    void Response::add_read_byte_count(std::size_t offset) {
        m_offset += std::ranges::min(offset, get_remaining_byte_count());
    }

    std::size_t Response::get_remaining_byte_count() const noexcept {
        return m_data.size() - m_offset;
    }

    bool Response::is_fully_sent() const noexcept {
        return get_remaining_byte_count() == 0;
    }

    const char* Response::get_data_pointer() const noexcept {
        return m_data.data() + m_offset;
    }
}
