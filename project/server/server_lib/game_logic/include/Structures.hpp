#pragma once

#include "Response.hpp"
#include "string"

// FEN notation
// https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
#define START_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"
#define BOARD_SIZE 64

struct coords {
    std::string str;
    int index;
    int x;
    int y;
};

struct move_response {
    move_status moveStatus;
    std::string table_fen;
    int move_from;
    int move_to;
};

coords data_to_coords(char letter, char number);
int str_to_index(char letter, char number);
coords data_to_coords(int index);
coords data_to_coords(std::string input);
std::string index_to_str(const int index);