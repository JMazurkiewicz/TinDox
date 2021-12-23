#include <netdb.h>
#pragma once

#include <cstdint>
#include <string>
#include <variant>

namespace tds::protocol {
    using FieldValue = std::variant<std::monostate, bool, std::uintmax_t, std::string>;

    enum class FieldValueType {
        none,
        boolean,
        integer,
        string,
    };
}
