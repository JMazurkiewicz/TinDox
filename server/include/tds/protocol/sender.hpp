#pragma once

#include "tds/linux/io_device.hpp"
#include "tds/protocol/response.hpp"

#include <queue>

namespace tds::protocol {
    class Sender {
    public:
        Sender();
        Sender(const Sender&) = delete;
        Sender& operator=(const Sender&) = delete;

        void set_device(linux::IoDevice& device);

        void add_response(Response response);
        bool has_responses() const noexcept;

        void send();

    private:
        linux::IoDevice* m_dev;
        std::queue<Response> m_responses;
    };
}
