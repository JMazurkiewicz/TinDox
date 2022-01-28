#ifndef TINDOX_CONSOLE_CLIENT_RETURN_EXCEPTION_H
#define TINDOX_CONSOLE_CLIENT_RETURN_EXCEPTION_H

#include <exception>
#include <string>

class return_exception : public std::exception {
public:
    explicit return_exception(std::string e_mess) : error_message(std::move(e_mess)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return error_message.c_str();
    }

private:
    const std::string error_message;
};


#endif //TINDOX_CONSOLE_CLIENT_RETURN_EXCEPTION_H
