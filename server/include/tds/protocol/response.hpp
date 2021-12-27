#pragma once

#include "tds/protocol/code.hpp"

namespace tds::protocol {
    class Response {
    public:
        explicit Response(std::string data);
        Response(const Response&) = delete;
        Response& operator=(const Response&) = delete;
        Response(Response&&) = default;
        Response& operator=(Response&&) = default;

        void add_read_byte_count(std::size_t offset);
        std::size_t get_remaining_byte_count() const noexcept;
        bool is_fully_sent() const noexcept;
        const char* get_data_pointer() const noexcept;

    private:
        std::string m_data;
        std::size_t m_offset;
    };
}
