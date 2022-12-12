#pragma once

#include <string>

#include "boost/json.hpp"

struct Request {
    virtual void parse(boost::json::object &requestData) = 0;
    bool is_valid() { return operation_result_; }

    virtual std::string toJSON() = 0;

    std::string basicRequest(std::string type) {
        boost::json::object object({{"type", type}});
        object["data"].emplace_object();
        return boost::json::serialize(object);
    }

    std::string dataRequest(std::string type, boost::json::object data) {
        boost::json::object object({{"type", type}});
        object["data"] = data;
        return boost::json::serialize(object);
    }

   protected:
    bool operation_result_ = true;
};
