#pragma once
#include <boost/asio/ip/tcp.hpp>
#include <map>
#include <string>

#include "BasicMenu.hpp"
#include "Handler.hpp"
#include "Request.hpp"
#include "Response.hpp"


class Router {
   public:
    struct Route {
        Request* request;
        Handler* handler;
        Response* response;
    };

    Router() = default;
    void addRoute(QueryType type, Route route);
    std::string process(const std::string& requestData,
                             User& user, BasicMenu& m_mainMenu);
    ~Router();

   private:
    std::map<QueryType, Route> routes_;
};