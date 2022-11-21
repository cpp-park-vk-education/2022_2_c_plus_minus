#pragma once  //  NOLINT

#include <unistd.h>

#include <pqxx/pqxx>
#include <queue>
#include <string>

#include "DbConnection.hpp"
#include "DbMethods.hpp"
#include "DbWorker.hpp"

template <typename T = pqxx::connection, class C = DBConnection<T>,
          class M = DBMethods<T, C>, class W = DbWorker<T, C, M>>
class DBManager {
    std::vector<std::unique_ptr<W>> workers_pool_;

   public:
    size_t workers_count_;
    M methods_;
    W *GetFreeWorker(const size_t idx);
    size_t Size() const noexcept;
    DBManager();
    ~DBManager() = default;
};

template <typename T>
class Singleton {
   private:
    T data_;

   public:
    static Singleton &GetInstance() {
        static Singleton _instance;
        return _instance;
    }

    T &GetData() { return data_; }

   private:
    Singleton() = default;
    Singleton(const Singleton &s) {}
    Singleton &operator=(const Singleton &s) { return s; }
    ~Singleton() {}
};
