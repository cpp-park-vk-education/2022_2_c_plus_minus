#pragma once

#include "Message.hpp"

// types of requests
// Add then other types
enum class ReqType {
    DEFAULT,
    GET_ID,

    USER_ENTER,
    USER_EXIT,
    // Types for handling rooms
    ROOM_CREATE,
    ROOM_JOIN,
    ROOM_EXIT,

    GAME_START,
    FIGURE_MOVE,

    NONE,
};

// functions for working with ReqType
std::string ReqToStr(const ReqType type);
ReqType StrToReq(const std::string& str);

class Request : public Message {
   public:
    ReqType type_ = ReqType::DEFAULT;
    std::string data_;
    void FromJSON(const std::string& json) override;
    void ToJSON(std::string& json) const override;
};
