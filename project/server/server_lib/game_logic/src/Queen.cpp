#include "Queen.hpp"

Queen::Queen(figure_color color) {
    Queen::isDead = 0;
    Queen::name = "Queen";
    Queen::color = color;
    Queen::FEN_name = (color == WHITE) ? "Q" : "q";
}