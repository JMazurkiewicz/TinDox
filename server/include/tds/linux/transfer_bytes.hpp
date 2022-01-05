#pragma once

#include "tds/linux/io_device.hpp"

#include <sys/types.h>

namespace tds::linux {
    ssize_t transfer_bytes(IoDevice& source, IoDevice& target, off64_t& offset, std::size_t count);
    ssize_t transfer_bytes(IoDevice& source, IoDevice& target, off64_t& offset, std::size_t count, std::errc& code);
}
