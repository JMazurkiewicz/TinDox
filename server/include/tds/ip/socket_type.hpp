#pragma once

#include <sys/socket.h>

namespace tds::ip {
    enum class SocketType {
        blocking = 0,
        nonblocking = SOCK_NONBLOCK,
    };
}
