#pragma once

#include <string>

#include "Response.hpp"

struct ErrorResponse : public Response {
    std::string toJSON() override { return "ERROR"; }
    void parse(const std::string &requestData) override {}
};