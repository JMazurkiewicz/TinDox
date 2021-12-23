#pragma once

#include <cstdint>

namespace tds::protocol {
    enum class InputInterpreterMode : std::uint8_t {
        protocol,
        download,
        upload,
    };
}
