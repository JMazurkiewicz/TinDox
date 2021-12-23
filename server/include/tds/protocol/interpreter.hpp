#pragma once

#include <span>
#include <vector>

namespace tds::protocol {
    class Interpreter {
    public:
        Interpreter() = default;
        Interpreter(const Interpreter&) = delete;
        Interpreter& operator=(const Interpreter&) = delete;

        /// @todo Temporary solution -- should not be movable
        Interpreter(Interpreter&&) = default;
        Interpreter& operator=(Interpreter&&) = default;

        void commit(std::span<const std::byte> bytes);

    private:

    };
}
