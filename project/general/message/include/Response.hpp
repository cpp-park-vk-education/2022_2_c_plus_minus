#pragma once

#include <boost/json.hpp>
#include <string>
#include "Base.hpp"

struct Response {
    QueryType resp_type {QueryType::UNDEFINED};
    std::string resp_data {};

    Response(){
        resp_type = QueryType::UNDEFINED;
        resp_data = {};
    }

    Response (QueryType type, std::string data){
        resp_type = type;
        resp_data = data;
    }

    virtual std::string toJSON() {
        boost::json::object object({{ "type", AsStringInternal(resp_type)},
                                    { "data", resp_data}});
        return boost::json::serialize(object) + separator;
    };

    virtual void parse(const std::string &requestData){
        boost::json::object json_object = boost::json::parse(requestData).get_object();
                try {
                    resp_type = AsQueryTypeInternal(json_object["type"].as_string().c_str());
                    resp_data = json_object["data"].as_string().c_str();
                } catch (...) {
                    // in some cases data is empty
                }
    }
    bool is_valid() { return operation_result_; }

   protected:
    bool operation_result_ = true;
};
