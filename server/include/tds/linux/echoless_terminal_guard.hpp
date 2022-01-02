#pragma once

namespace tds::linux {
    class EcholessTerminalGuard {
    public:
        EcholessTerminalGuard();
        ~EcholessTerminalGuard();

        EcholessTerminalGuard(const EcholessTerminalGuard&) = delete;
        EcholessTerminalGuard& operator=(const EcholessTerminalGuard&) = delete;
    };
}
