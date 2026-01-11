// animations.hpp
#pragma once
#include <array>
#include <string>
#include <unordered_set>
#include "hardware.hpp"
#include "chess.hpp"


struct Animation {
    std::array<LedColor, 64> leds;
    int display_time_ms;
};


std::string playAnimations(std::atomic<bool>&, const std::vector<Animation>&);
void playWinnerAnimation(ChessGame);
void playStartUpAnimation(void);