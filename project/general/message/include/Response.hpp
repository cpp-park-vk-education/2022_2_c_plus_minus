#pragma once

#include <boost/json.hpp>
#include <string>

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

struct Response {
    virtual std::string toJSON() = 0;
    virtual void parse(const std::string &requestData) = 0;
    bool is_valid() { return operation_result_; }

   protected:
    bool operation_result_ = true;
};
