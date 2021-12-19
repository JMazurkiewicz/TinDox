#pragma once

#include "tds/user/permissions.hpp"

#include <ostream>
#include <string>
#include <string_view>

namespace tds::user {
    class UserRecordData {
    public:
        explicit UserRecordData(std::string username, std::string password_hash, Permissions perms);

        std::string_view get_username() const noexcept;
        std::string_view get_password_hash() const noexcept;
        Permissions get_permissions() const noexcept;

        bool check_password(const std::string& password) const;
        bool check_password(const char* password) const;

    private:
        std::string m_username;
        std::string m_password_hash;
        Permissions m_perms;
    };

    UserRecordData make_user_record_data(std::string_view str);

    std::ostream& operator<<(std::ostream& stream, const UserRecordData& record);
}
