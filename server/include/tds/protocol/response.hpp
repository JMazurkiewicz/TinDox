#pragma once

namespace tds::protocol {
    class Response {
    public:
        Response();

        Response(const Response&) = delete;
        Response& operator=(const Response&) = delete;

        Response(Response&&) = default;
        Response& operator=(Response&&) = default;

    private:

    };
}
