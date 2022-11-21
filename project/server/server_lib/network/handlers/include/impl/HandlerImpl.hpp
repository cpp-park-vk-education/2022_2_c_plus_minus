#pragma once

#include <memory>

#include "Handler.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "SessionImpl.hpp"

class HandlerImpl : public Handler {
   public:
    HandlerImpl(const Request* request, SessionImpl* session);
    virtual ~HandlerImpl() = default;
    void Run() override;

   protected:
    void Respond() override;
    bool Check() override;

   protected:
    const Request* request_ = nullptr;
    SessionImpl* session_ = nullptr;
    Response response_;
};
