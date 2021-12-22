#pragma once

#include "tds/linux/event_type.hpp"

namespace tds::linux {
    struct EpollEvent {
        int fd;
        EventType events;
    };
}
