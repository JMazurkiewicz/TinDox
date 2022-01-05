#include "tds/linux/transfer_bytes.hpp"

#include "tds/linux/linux_error.hpp"

#include <sys/sendfile.h>

namespace tds::linux {
    ssize_t transfer_bytes(IoDevice& source, IoDevice& target, off64_t& offset, std::size_t count) {
        std::errc code = {};
        const ssize_t result = transfer_bytes(source, target, offset, count);
        if(code != std::errc{}) {
            throw LinuxError{"sendfile64(2)"};
        } else {
            return result;
        }
    }

    ssize_t transfer_bytes(IoDevice& source, IoDevice& target, off64_t& offset, std::size_t count, std::errc& code) {
        const ssize_t result = sendfile64(target.get_fd(), source.get_fd(), &offset, count);
        if(count == -1) {
            code = static_cast<std::errc>(errno);
        }
        return count;
    }
}
