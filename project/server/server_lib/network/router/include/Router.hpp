#pragma once
#include <boost/asio/ip/tcp.hpp>
#include <map>
#include <string>

#include "BasicMenu.hpp"
#include "ClientData.hpp"
#include "Handler.hpp"
#include "Request.hpp"
#include "Response.hpp"

struct Route {
    Request* request;
    Handler* handler;
    Response* response;
};

class Router {
   public:
    Router() = default;
    void addRoute(std::string name, Route route);
    std::string process(const std::string& requestData,
                        ClientData& m_clientData, BasicMenu& m_mainMenu);
    ~Router();

   private:
    std::map<std::string, Route> routes_;
};