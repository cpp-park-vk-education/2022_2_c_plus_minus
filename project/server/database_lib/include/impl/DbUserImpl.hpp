#pragma once

#include "DbUser.hpp"

static enum result_registration {
    NOT_REGISTRATION = 1,
    ERROR_REGISTRATION
} REG;

static enum result_authentication {
    NOT_AUTHENTICATION = 3,
    ERROR_AUTHENTICATION
} AUTH;

static enum result_get_user_id {
    NOT_GET_USER_ID = 6,
    ERROR_GET_USER_ID
} GET_USER_ID;

static enum result_get_user_nickname {
    NOT_GET_USER_NICKNAME = 9,
    ERROR_GET_USER_NICKNAME
} GET_NICKNAME;

static enum result_delete_user {
    NOT_DELETE_USER = 12,
    ERROR_DELETE_USER
} USER_DELETE;

static enum result_update_user_nick {
    NOT_UPDATE_USER_NICK = 15,
    ERROR_UPDATE_USER_NICK
} UPDATE_USER_NICK;

static enum result_update_user_password {
    NOT_UPDATE_USER_PASSWORD = 18,
    ERROR_UPDATE_USER_PASSWORD
} UPDATE_USER_PASSWORD;

template <class C>
class DbUserImpl : public DbUser<C> {
   public:
    int GetId(const User &user, std::string *user_id,
              C *connection) const override;

    int GetNickname(const User &user, std::string *nickname,
                    C *connection) const override;

    int UpdatePassword(const std::string &user_id,
                       C *connection) const override;

    int UpdateNick(const std::string &user_id, C *connection) const override;

    int Delete(const User &user, C *connection) const override;

    int Registration(const User &user, std::string *new_user_id,
                     C *connection) const override;

    int Authentication(const User &user, std::string *user_id,
                       C *connection) const override;

    DbUserImpl() = default;
    ~DbUserImpl() = default;
};