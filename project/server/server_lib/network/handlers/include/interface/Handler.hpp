#pragma once

#include "BasicMenu.hpp"
#include "ClientData.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Handler {
   public:
    Handler() = default;
    virtual void process(const Request* request, Response* response,
                         ClientData& m_clientData, BasicMenu& m_mainMenu) = 0;
    virtual ~Handler() = default;
};
