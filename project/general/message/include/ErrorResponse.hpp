#pragma once

#include <string>

#include "Response.hpp"

struct ErrorResponse : public Response {
    ErrorResponse() {}
    std::string toJSON() override {
        boost::json::object object({{"ERROR", "DSC"}});
        return boost::json::serialize(object);
    }
    void parse(const std::string &requestData) override {}
};