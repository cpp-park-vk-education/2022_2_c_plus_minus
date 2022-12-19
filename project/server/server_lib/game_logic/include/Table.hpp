#pragma once

#include "Bishop.hpp"
#include "Figure.hpp"
#include "GameResponse.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Pawn.hpp"
#include "Queen.hpp"
#include "Rock.hpp"
#include "Structures.hpp"

class ChessTable {
   public:
    ChessTable();
    ChessTable(const ChessTable &mainTable);
    Figure **get_table() const;
    void set_table(Figure **input_table);
    void kill_figure(int index);
    move_status move_figure(int from, int to);
    void make_move(int from, int to);
    move_status is_legal(int from, int to);
    bool Check_mate(int King_ind);
    void set_enpassant(char letter, char number);
    bool check_en_passant(int from, int to);
    bool Rock_move(figure_color color, coords from, coords to);
    bool Bishop_move(figure_color color, coords from, coords to);
    bool Queen_move(figure_color color, coords from, coords to);
    bool get_hit_table(figure_color color, coords to);
    bool check_cast(Figure *fig_actor, figure_color enemy, coords from,
                    coords to);
    bool check_draw(figure_color enemy_color);
    Figure *get_figure(int index);
    ~ChessTable();
    int Black_king_ind = -1;
    int White_king_ind = -1;
    void FEN_parser(std::string FEN);
    std::string get_FEN() const;
    figure_color turn;

   private:
    void delete_table();
    Figure **table;
    std::string whiteDeadFigures = "";
    std::string blackDeadFigures = "";
    std::vector<int> black_enpassant;
    std::vector<int> white_enpassant;
};
