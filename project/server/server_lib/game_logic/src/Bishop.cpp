#include "Bishop.hpp"

Bishop::Bishop(figure_color color) {
    Bishop::isDead = 0;
    Bishop::name = "Bishop";
    Bishop::color = color;
    Bishop::FEN_name = (color == WHITE) ? "B" : "b";
}
