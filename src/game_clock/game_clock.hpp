// game_clock.hpp
#pragma once
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "hardware.hpp"
#include "chess.hpp"


struct ClockSetting
{
    std::atomic<bool> active{true}; // True if a winner hasn't been declared
    std::atomic<Color> player_turn{Color::White}; // Players turn do determine the active clock
    std::atomic<int> time_white; // Time remaining on whites clock in ms
    std::atomic<int> time_black; // Same for black
    std::atomic<int> extra_time; // Extra time in ms to add after each move
    std::mutex mtx; // Mutex to synchronize
    ClockSetting(int game_time_min, int extra_time_sec)
                : time_white(game_time_min * 60000), time_black(game_time_min * 60000), extra_time(extra_time_sec * 1000) {}
};


void startGameClock(ClockSetting&, std::atomic<bool>&);
