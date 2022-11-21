#pragma once

#include "Message.hpp"
#include "Request.hpp"

enum class StatusCode {
    SUCCESS = 200,
    METHOD_NOT_ALLOWED = 405,
    INTERNAL_SERVER_ERROR = 500,
};

class Response : public Message {
   public:
    ReqType type_ = ReqType::DEFAULT;
    StatusCode st_code_ = StatusCode::INTERNAL_SERVER_ERROR;
    std::string data_;
    void FromJSON(const std::string& json) override;
    void ToJSON(std::string& json) const override;
};
