#pragma once

#include "tds/protocol/code.hpp"
#include "tds/protocol/response.hpp"

#include <vector>

namespace tds::protocol {
    class ResponseBuilder {
    public:
        ResponseBuilder() = default;
        ResponseBuilder(const ResponseBuilder&) = delete;
        ResponseBuilder& operator=(const ResponseBuilder&) = delete;

        void set_code(Code code);
        void set_command_name(std::string command_name);
        void add_line(std::string_view line);

        std::string get_response();

    private:
        Code m_code;
        std::string m_command_name;
        std::string m_lines;
    };
}
