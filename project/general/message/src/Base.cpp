#include "Base.hpp"

std::string AsStringInternal(const QueryType ty) {
    static std::map<QueryType, std::string> mapped = {
        {QueryType::UNDEFINED, "undefined"},
        {QueryType::CREATE_ROOM, "create-room"},
        {QueryType::GET_ROOMS, "get-rooms"},
        {QueryType::AUTHORISE, "authorise"},
        {QueryType::ENTER_ROOM, "enter-room"},
        {QueryType::ON_ENTER_ROOM, "on-enter-room"},
        {QueryType::LEAVE_ROOM, "leave-room"},
        {QueryType::START_GAME, "start-game"},
        {QueryType::MOVE_FIGURE, "move-figure"}};
    return mapped.at(ty);
}

QueryType AsQueryTypeInternal(const std::string& str) {
    static std::map<std::string, QueryType> mapped = {
        {"undefined", QueryType::UNDEFINED},
        {"create-room", QueryType::CREATE_ROOM},
        {"get-rooms", QueryType::GET_ROOMS},
        {"authorise", QueryType::AUTHORISE},
        {"enter-room", QueryType::ENTER_ROOM},
        {"on-enter-room", QueryType::ON_ENTER_ROOM},
        {"leave-room", QueryType::LEAVE_ROOM},
        {"start-game", QueryType::START_GAME},
        {"move-figure", QueryType::MOVE_FIGURE}};
    return mapped.at(str);
}