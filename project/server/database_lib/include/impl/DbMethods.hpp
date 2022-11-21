#pragma once  //  NOLINT

#include "DbConnection.hpp"
#include "DbTaskImpl.hpp"
#include "DbUserImpl.hpp"
#include "pqxx/pqxx"

template <typename T = pqxx::connection, class C = DBConnection<T>>
class DBMethods {
   public:
    DbUserImpl<C> User;
    DbTask<C> Task;

    DBMethods() = default;

    ~DBMethods() = default;
};
