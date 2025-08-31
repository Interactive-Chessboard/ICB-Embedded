// clock_struct.hpp
#pragma once
#include <mutex>
#include <atomic>
struct ClockSetting
{
    std::atomic<char> player_turn{'w'}; // Players turn do determine the active clock
    std::atomic<int> time_white; // Time remaining on whites clock in 1/100th of a second
    std::atomic<int> time_black; // Same for black
    std::atomic<int> extra_time; // Extra time in 1/100th of a second to add after each move
    std::atomic<bool> white_start_timer{false}; // Start the timer after whites first move
    std::atomic<bool> black_start_timer{false}; // Same for black
    std::atomic<bool> active{true}; // True if a winner hasn't been declared
    std::atomic<char> winner{' '}; // Winner declared if a player has lost on time
    std::mutex mtx; // Mutex to synchronize
    ClockSetting(int game_time, int extra_time)
                : time_white(game_time), time_black(game_time), extra_time(extra_time) {}
};