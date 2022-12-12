#pragma once

#include "Figure.hpp"

class Rock : public Figure {
   public:
    int wasPawn = 0;
    Rock(figure_color color);
};