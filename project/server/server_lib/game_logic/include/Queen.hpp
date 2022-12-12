#pragma once
#include "Figure.hpp"
class Queen : public Figure {
   public:
    int wasPawn = 0;
    Queen(figure_color color);
};