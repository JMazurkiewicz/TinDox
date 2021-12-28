#pragma once

#include "tds/protocol/request.hpp"

#include <span>
#include <string_view>
#include <vector>

namespace tds::protocol {
    class ProtocolInterpreter {
    public:
        static constexpr std::size_t max_line_length = 2048;
        static constexpr std::size_t max_line_count = 16;

        static constexpr char line_separator = '\n';
        static constexpr char field_separator = ':';
        static constexpr std::string_view allowed_whitespaces = " \t";

        ProtocolInterpreter();
        ProtocolInterpreter(const ProtocolInterpreter&) = delete;
        ProtocolInterpreter& operator=(const ProtocolInterpreter&) = delete;

        void add_bytes(std::span<const char> input);

        bool has_available_requests();
        [[nodiscard]] std::vector<Request> get_requests();

    private:
        void add_line(std::string line);
        void check_lines();
        void add_response();

        std::vector<std::string> m_lines;
        std::vector<Request> m_requests;

        bool m_last_line_complete : 1;
        int m_last_checked_line;
    };
}
