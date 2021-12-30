#pragma once

#include "tds/protocol/protocol_code.hpp"

#include <stdexcept>

namespace tds::protocol {
    const std::error_category& get_tdp_category();

    class ProtocolError : public std::runtime_error {
    public:
        explicit ProtocolError(ProtocolCode code);
        explicit ProtocolError(ProtocolCode code, const std::string& message);
        explicit ProtocolError(ProtocolCode code, const char* message);

        ProtocolCode get_code() const noexcept;

    private:
        ProtocolCode m_code;
    };
}
