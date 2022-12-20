#pragma once

#include "map"
#include "string"

const std::string separator = "\r\n\r\n";

enum figure_color: int8_t { WHITE = 1, BLACK = -1 };

enum move_status {
    MOVE_OK,
    MOVE_EAT,
    MOVE_CAST,
    MOVE_PAWN_UPGRADE,
    MOVE_SET_PAWN_UPGRADE,
    MOVE_CHECK_WHITE,
    MOVE_CHECKMATE_WHITE,
    MOVE_CHECK_BLACK,
    MOVE_CHECKMATE_BLACK,
    MOVE_DRAW,
    MOVE_ERROR,
    MOVE_ENPASSANT,
    MOVE_SET_ENPASSANT,
    WHITE_LEAVE,
    BLACK_LEAVE
};

enum class QueryType {
    // default or error
    UNDEFINED,
    CREATE_ROOM,
    GET_ROOMS,
    AUTHORISE,
    ENTER_ROOM,
    ON_ENTER_ROOM,
    LEAVE_ROOM,
    START_GAME,
    MOVE_FIGURE
};

std::string AsStringInternal(const QueryType ty);
QueryType AsQueryTypeInternal(const std::string& str);