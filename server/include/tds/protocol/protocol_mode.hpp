#pragma once

#include <cstdint>

namespace tds::protocol {
    enum class ProtocolMode : std::uint8_t {
        command,
        download,
        upload,
    };
}
