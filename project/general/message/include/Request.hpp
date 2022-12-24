#pragma once

#include <string>

#include "Base.hpp"
#include "boost/json.hpp"

struct Request {
    QueryType req_type{QueryType::UNDEFINED};
    boost::json::object req_data{};

    virtual void parse(boost::json::object &json) {
        try {
            req_type = AsQueryTypeInternal(json["type"].as_string().c_str());
            req_data = json["data"].as_object();
        } catch (...) {
        }
    };
    bool is_valid() { return operation_result_; }

    virtual std::string toJSON() {
        boost::json::object object({{"type", AsStringInternal(req_type)}});
        object["data"] = boost::json::serialize(req_data);
        return boost::json::serialize(object) + separator;
    };

    std::string basicRequest(QueryType type) {
        boost::json::object object({{"type", AsStringInternal(type)}});
        object["data"].emplace_object();
        return boost::json::serialize(object) + separator;
    }

    std::string dataRequest(QueryType type, boost::json::object data) {
        boost::json::object object({{"type", AsStringInternal(type)}});
        object["data"] = data;
        return boost::json::serialize(object) + separator;
    }

   protected:
    bool operation_result_ = true;
};
