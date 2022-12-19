#pragma once

#include "BasicMenu.hpp"
#include "User.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Handler {
   public:
    Handler() = default;
    virtual void process(const Request* request, Response* response,
                         User& user, BasicMenu& menu) = 0;
    virtual ~Handler() = default;
};
