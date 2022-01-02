#include "tds/linux/echoless_terminal_guard.hpp"

#include "tds/linux/terminal.hpp"

namespace tds::linux {
    EcholessTerminalGuard::EcholessTerminalGuard() {
        Terminal::set_stdin_echo(false);
    }

    EcholessTerminalGuard::~EcholessTerminalGuard() {
        Terminal::set_stdin_echo(true);
    }
}
