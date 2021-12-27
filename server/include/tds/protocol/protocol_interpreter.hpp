#pragma once

#include "tds/protocol/response.hpp"

#include <span>
#include <vector>

namespace tds::protocol {
    class ProtocolInterpreter {
    public:
        ProtocolInterpreter() = default;
        ProtocolInterpreter(const ProtocolInterpreter&) = delete;
        ProtocolInterpreter& operator=(const ProtocolInterpreter&) = delete;

        void add_bytes(std::span<const char> input);

        bool has_available_responses();
        std::vector<Response> get_responses();

    private:
        std::vector<char> m_input;
        std::vector<Response> m_responses;
    };
}
