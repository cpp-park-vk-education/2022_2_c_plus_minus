#pragma once
#include "Response.hpp"

struct AuthResponse : public Response {
    AuthResponse() = default;
    std::string message;
    int status;

    std::string toJSON() override {
        boost::json::object object({{"status", status}, {"message", message}});
        return boost::json::serialize(object);
    }

    void parse(const std::string &requestData) override {
        boost::json::error_code ec;
        boost::json::value parsedData = boost::json::parse(requestData, ec);

        if (ec) {
            operation_result_ = false;
            return;
        }
        operation_result_ = true;

        status = parsedData.at("status").to_number<int>();
        message = boost::json::value_to<std::string>(parsedData.at("message"));
    }
};

struct GameResponse : public Response {
    int moveStatus;
    std::string tableFEN;
    int moveFrom;
    int moveTo;
    std::string move_str;

    GameResponse() = default;

    std::string toJSON() override {
        boost::json::object object({{"moveStatus", moveStatus},
                                    {"tableFEN", tableFEN},
                                    {"moveFrom", moveFrom},
                                    {"moveTo", moveTo},
                                    {"move-str", move_str}});

        return boost::json::serialize(object);
    }

    void parse(const std::string &requestData) override {
        boost::json::error_code ec;
        boost::json::value parsedData = boost::json::parse(requestData, ec);

        if (ec) {
            operation_result_ = false;
            return;
        }
        operation_result_ = true;

        moveStatus = parsedData.at("moveStatus").to_number<int>();
        tableFEN =
            boost::json::value_to<std::string>(parsedData.at("tableFEN"));
        moveFrom = parsedData.at("moveFrom").to_number<int>();
        moveTo = parsedData.at("moveTo").to_number<int>();
        move_str =
                boost::json::value_to<std::string>(parsedData.at("move-str"));
    }
};