#pragma once

#include <cstdint>

namespace tds::protocol {
    enum class InterpreterMode : std::uint8_t {
        protocol,
        download,
        upload,
    };
}
