#include "tds/linux/terminal.hpp"

#include "tds/linux/linux_error.hpp"

#include <termios.h>
#include <unistd.h>

namespace tds::linux {
    void Terminal::set_stdin_echo(bool visibility) {
        termios term;
        if(tcgetattr(STDIN_FILENO, &term) == -1) {
            throw LinuxError{"tcgetattr(3)"};
        }

        if(visibility) {
            term.c_lflag |= ECHO;
        } else {
            term.c_lflag &= ~ECHO;
        }

        if(tcsetattr(STDIN_FILENO, 0, &term) == -1) {
            throw LinuxError{"tcsetattr(3)"};
        }
    }
}
