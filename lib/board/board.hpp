// board.hpp
#pragma once
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <atomic>
#include <array>

struct LedColor {
    u_int8_t red;
    u_int8_t green;
    u_int8_t blue;
    LedColor(int red = 0, int green = 0, int blue = 0) : 
             red(red), green(green), blue(blue) {}
};


struct Animation {
    std::array<LedColor, 64> leds;
    int display_time_ms;
};


namespace Board
{
// board
uint64_t getBoardArr();

// lights
void setLed(std::array<LedColor, 64>);
void clearLed(void);
std::string playAnimations(std::atomic<bool>&, const std::vector<Animation>&);

// screen
void setTime(int white_time_ms, int black_time_ms);

};