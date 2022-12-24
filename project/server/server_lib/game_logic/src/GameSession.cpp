#include "GameSession.hpp"

#include <cstdlib>
#include <iostream>  //DEBUG
#include <string>
#include <vector>

GameSession::GameSession(const std::string &id,
                         const std::string &whitePlayerId,
                         const std::string &blackPlayerId,
                         const std::string &FEN)
    : id(id), whitePlayerId(whitePlayerId), blackPlayerId(blackPlayerId) {
    GameSession::table.FEN_parser(FEN);
}

void GameSession::create_error_response(move_response &result) const {
    result.moveStatus = MOVE_ERROR;
    result.table_fen = GameSession::table.get_FEN();
    result.move_from = -1;
    result.move_to = -1;
}

move_response GameSession::makeMove(std::string move,
                                    const std::string &player_id) {
    move_response result;
    if (!GameSession::beginned || GameSession::ended)  // если игра закончилась
    {
        GameSession::create_error_response(result);
        return result;
    }
    // проверяем id
    if (((GameSession::table.turn == WHITE) &&
         (player_id != GameSession::whitePlayerId)) ||
        ((GameSession::table.turn == BLACK) &&
         (player_id != GameSession::blackPlayerId))) {
        GameSession::create_error_response(result);
        return result;
    }
    std::cout << GameSession::table.turn << std::endl;  // DEBUG
    // определяем фигуру
    int from = str_to_index(move.at(0), move.at(1));
    int to = str_to_index(move.at(2), move.at(3));

    if ((table.get_figure(from) == nullptr) || (to == -1) ||
        (from == to)) {  // если координаты не верны
        GameSession::create_error_response(result);
        return result;
    }
    if (GameSession::table.turn !=
        table.get_figure(from)->color) {  // ходит не тот цвет
        GameSession::create_error_response(result);
        return result;
    }
    std::cout << GameSession::table.turn << std::endl;  // DEBUG
    // проверяем на ничью
    // std::cout<<"DRAW"<<std::endl;//DEBUG

    move_status move_result = GameSession::table.move_figure(from, to);

    if (move_result == MOVE_ERROR) {
        GameSession::create_error_response(result);
        std::cout << "ERROR" << std::endl;  // DEBUG
        return result;
    }
    result.moveStatus = move_result;
    result.table_fen = GameSession::table.get_FEN();
    result.move_from = from;
    result.move_to = to;
    GameSession::table.turn =
        (GameSession::table.turn == WHITE) ? BLACK : WHITE;
    return result;
    // возвращаем данные
}

GameSession::~GameSession(){

};

void ChessTable::set_enpassant(char letter, char number) {
    int index = str_to_index(letter, number);
    if ((index < 49) && (index > 40)) {
        index -= 8;
        if (table[index] != NULL) {
            if (table[index]->name == "Pawn") {
                Pawn *cur = static_cast<Pawn *>(table[index]);
                cur->enPassant = true;
            }
        }
    }

    if ((index < 33) && (index > 24)) {
        index += 8;
        if (table[index] != NULL) {
            if (table[index]->name == "Pawn") {
                Pawn *cur = static_cast<Pawn *>(table[index]);
                cur->enPassant = true;
            }
        }
    }
}

bool GameSession::end_game(move_status why) {
    if ((why == MOVE_DRAW) || (why == MOVE_CHECKMATE_BLACK) ||
        (why == MOVE_CHECKMATE_WHITE) || (why == WHITE_LEAVE) ||
        (why == BLACK_LEAVE)) {
        beginned = 0;
        ended = 1;
        gameResult = why;
    }
}
