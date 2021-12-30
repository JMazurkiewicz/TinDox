#pragma once

#include "tds/protocol/request.hpp"

#include <optional>
#include <span>
#include <string_view>
#include <vector>

namespace tds::protocol {
    class ProtocolInterpreter {
    public:
        static constexpr std::size_t max_line_length = 2048;
        static constexpr std::size_t max_line_count = 16;
        static constexpr char line_separator = '\n';

        ProtocolInterpreter();
        ProtocolInterpreter(const ProtocolInterpreter&) = delete;
        ProtocolInterpreter& operator=(const ProtocolInterpreter&) = delete;

        [[nodiscard]] std::span<const char> commit_bytes(std::span<const char> bytes);

        [[nodiscard]] bool has_available_request() const noexcept;
        [[nodiscard]] Request get_request();

    private:
        void commit_line(std::span<const char> line);
        void check_for_protocol_errors();
        void commit_request();

        std::vector<std::string> m_lines;
        std::optional<Request> m_request;

        bool m_last_line_complete : 1;
    };
}
