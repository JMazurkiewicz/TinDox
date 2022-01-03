#pragma once

#include "tds/protocol/execution/exit.hpp"

namespace tds::protocol::execution {
    class Bye : public Exit {
    public:
        static constexpr std::string_view name = "bye";
    };
}
