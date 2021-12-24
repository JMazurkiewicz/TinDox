#pragma once

#include "tds/protocol/code.hpp"

namespace tds::protocol {
    class Response {
    public:
        explicit Response(Code code, std::string_view name);
        Response(const Response&) = delete;
        Response& operator=(const Response&) = delete;
        Response(Response&&) = default;
        Response& operator=(Response&&) = default;

        void add_line(std::string_view line);
        void finish();

        std::size_t get_offset() const noexcept;
        std::size_t get_remaining_byte_count() const noexcept;
        void add_offset(std::size_t offset);
        bool is_fully_sent() const noexcept;
        const char* get_current_data() const noexcept;

    private:
        static std::string make_response_header(Code code, std::string_view name);

        std::string m_data;
        bool m_finished;
        std::size_t m_offset;
    };
}
