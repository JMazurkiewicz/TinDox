#pragma once

namespace tds::linux {
    class Terminal {
    public:
        static void set_stdin_echo(bool visibility);
    };
}
