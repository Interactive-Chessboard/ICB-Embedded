// board.hpp
#pragma once
#include "structs.hpp"

struct LedColor {
    u_int8_t red;
    u_int8_t green;
    u_int8_t blue;
    LedColor(int red = 0, int green = 0, int blue = 0) : 
             red(red), green(green), blue(blue) {}
};


namespace Board
{
// board
u_int64_t getBoardArr();

// lights
void setLed(std::array<LedColor, 64>);
void clearLed(void);

};