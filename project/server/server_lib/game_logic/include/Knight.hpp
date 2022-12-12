#pragma once
#include "Figure.hpp"
class Knight : public Figure {
   public:
    int wasPawn = 0;
    Knight(figure_color color);
};
