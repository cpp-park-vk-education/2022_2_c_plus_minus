#pragma once

#include "User.hpp"

template <class C>
class DbUser {
   public:
    virtual int GetId(const User &user, std::string *user_id,
                      C *connection) const = 0;

    virtual int GetNick(const User &user, std::string *nick,
                        C *connection) const = 0;

    virtual int GetPassword(const User &user, std::string *nick,
                            C *connection) const = 0;

    virtual int UpdatePassword(const std::string &user_id,
                               C *connection) const = 0;

    virtual int UpdateNick(const std::string &user_id, C *connection) const = 0;

    virtual int Delete(const User &user, C *connection) const = 0;

    virtual int Registration(const User &user, std::string *new_user_id,
                             C *connection) const = 0;

    virtual int Authentication(const User &user, std::string *user_id,
                               C *connection) const = 0;

    virtual ~DbUser() = default;
};
