#include "Base.hpp"

std::string ColorToStr(figure_color color) {
    if (color == figure_color::WHITE) {
        return "white";
    }
    return "black";
}

figure_color StrToColor(std::string color) {
    if (color == "white") {
        return figure_color::WHITE;
    }
    return figure_color::BLACK;
}
std::string MoveStatusAsString(move_status mv) {
    static std::map<move_status, std::string> mapped = {
        {move_status::MOVE_OK, "Success"},
        {move_status::MOVE_EAT, "Eat figure"},
        {move_status::MOVE_CAST, "Cast"},
        {move_status::MOVE_PAWN_UPGRADE, "Pawn Upgrade"},
        {move_status::MOVE_SET_PAWN_UPGRADE, "Pawn set upgrade"},
        {move_status::MOVE_CHECK_WHITE, "Check white"},
        {move_status::MOVE_CHECKMATE_WHITE, "Check Mate white"},
        {move_status::MOVE_CHECK_BLACK, "Check Black"},
        {move_status::MOVE_CHECKMATE_BLACK, "Check Mate Black"},
        {move_status::MOVE_DRAW, "Draw"},
        {move_status::MOVE_ERROR, "Move error"},
        {move_status::MOVE_ENPASSANT, "Move enpassant"},
        {move_status::MOVE_SET_ENPASSANT, "Set enpassant"},
        {move_status::WHITE_LEAVE, "White leave"},
        {move_status::BLACK_LEAVE, "Black leave"}};
    return mapped.at(mv);
}
// move_status StringAsMoveStatus(const std::string& str);

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