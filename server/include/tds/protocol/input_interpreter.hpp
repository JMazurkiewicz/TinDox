#pragma once

#include "tds/protocol/input_interpreter_mode.hpp"

namespace tds::protocol {
    class InputInterpreter {
    public:
        InputInterpreter();
        InputInterpreter(const InputInterpreter&) = delete;
        InputInterpreter& operator=(const InputInterpreter&) = delete;

    private:
        InputInterpreterMode m_mode;
    };
}
