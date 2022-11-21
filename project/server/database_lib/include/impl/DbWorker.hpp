#pragma once

#include "DbConnection.hpp"
#include "DbMethods.hpp"
#include "pqxx/pqxx"

template <typename T = pqxx::connection, class C = DBConnection<T>,
          class M = DBMethods<T, C>>
class DbWorker {
   public:
    C connection;
    const M &db_methods;
    explicit DbWorker(const M &db_methods) : db_methods(db_methods) {}
    ~DbWorker() = default;
};
