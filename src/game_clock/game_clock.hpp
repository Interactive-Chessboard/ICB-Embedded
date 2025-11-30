// game_clock.hpp
#include <mutex>
#include <atomic>
#include "board.hpp"


enum class ClockColor {
    White,
    Black,
    Nil
};


struct ClockSetting
{
    std::atomic<bool> active{true}; // True if a winner hasn't been declared
    std::atomic<ClockColor> player_turn{ClockColor::White}; // Players turn do determine the active clock
    std::atomic<int> time_white; // Time remaining on whites clock in ms
    std::atomic<int> time_black; // Same for black
    std::atomic<int> extra_time; // Extra time in ms to add after each move
    std::atomic<bool> white_start_timer{false}; // Start the timer after whites first move
    std::atomic<bool> black_start_timer{false}; // Same for black
    std::mutex mtx; // Mutex to synchronize
    ClockSetting(int game_time_min, int extra_time_sec)
                : time_white(game_time_min * 6000), time_black(game_time_min * 6000), extra_time(extra_time + 100) {}
};


void game_clock(ClockSetting &clock_settings);