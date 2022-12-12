#pragma once

#include "Response.hpp"
#include "Room.hpp"

struct CreateRoomResponse : public Response {
    int status;
    CreateRoomResponse() = default;
    void parse(const std::string &requestData) override {}
    std::string toJSON() override {
        boost::json::object object({{"status", status}});

        return boost::json::serialize(object);
    }
};

struct EnterRoomResponse : public Response {
    int status;

    EnterRoomResponse() = default;
    void parse(const std::string &requestData) override {}
    std::string toJSON() override {
        boost::json::object object({{"status", status}});

        return boost::json::serialize(object);
    }
};

struct LeaveRoomResponse : public Response {
    int status;

    LeaveRoomResponse() = default;
    void parse(const std::string &requestData) override {}
    std::string toJSON() override {
        boost::json::object object({{"status", status}});

        return boost::json::serialize(object);
    }
};

struct GetRoomsResponse : public Response {
    int status;
    std::map<std::string, Room *> rooms;

    GetRoomsResponse() = default;
    void parse(const std::string &requestData) override {}
    std::string toJSON() override {
        boost::json::object object({{"status", status}});
        boost::json::array arr;
        for (auto el : rooms) {
            boost::json::object room({{el.first.data(), el.second->getName()}});
            arr.push_back(room);
        }
        object["rooms"] = arr;
        return boost::json::serialize(object);
    }
};

struct StartGameResponse : public Response {
    int status;
    void parse(const std::string &requestData) override {}
    StartGameResponse() = default;
    std::string toJSON() override {
        boost::json::object object({{"status", status}});

        return boost::json::serialize(object);
    }
};
