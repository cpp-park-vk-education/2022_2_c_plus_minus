#include "Pawn.hpp"
Pawn::Pawn(figure_color color) {
    Pawn::isDead = 0;
    Pawn::name = "Pawn";
    Pawn::color = color;
    Pawn::FEN_name = (color == WHITE) ? "P" : "p";
}
