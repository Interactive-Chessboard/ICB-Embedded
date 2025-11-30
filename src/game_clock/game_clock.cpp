// game_clock.cpp
#include <thread>
#include <chrono>
#include <atomic>
#include "game_clock.hpp"


void game_clock(ClockSetting &clock_settings)
{
    using clock = std::chrono::steady_clock;
    constexpr std::chrono::milliseconds interval(10);
    auto start = clock::now();
    int i = 0;
    ClockColor previous_iteration = ClockColor::White;
    while(clock_settings.active.load())
    {
        auto iteration_start = start + i * interval;

        { // Locks all the logic to avoid race conditions
            std::lock_guard<std::mutex> lock(clock_settings.mtx);

            // Start clock after one move
            if (clock_settings.player_turn.load() == ClockColor::Black)
            {
                clock_settings.white_start_timer.store(true);
            }
            if (clock_settings.white_start_timer.load() && 
                clock_settings.player_turn.load() == ClockColor::White)
            {
                clock_settings.black_start_timer.store(true);
            }

            // Decrement timer
            if (clock_settings.white_start_timer.load() && 
                clock_settings.player_turn.load() == ClockColor::White)
            {
                clock_settings.time_white.fetch_sub(1);
            }
            else if (clock_settings.black_start_timer.load() && 
                clock_settings.player_turn.load() == ClockColor::Black)
            {
                clock_settings.time_black.fetch_sub(1);
            }

            // Add extra time if changed turn
            if (clock_settings.white_start_timer.load() && 
                clock_settings.player_turn.load() == ClockColor::White && 
                previous_iteration != clock_settings.player_turn.load())
            {
                clock_settings.time_white.fetch_add(clock_settings.extra_time.load());
            }
            else if (clock_settings.black_start_timer.load() && 
                clock_settings.player_turn.load() == ClockColor::Black && 
                previous_iteration != clock_settings.player_turn.load())
            {
                clock_settings.time_black.fetch_add(clock_settings.extra_time.load());
            }

            // Verify clocks reached 0
            if (clock_settings.time_white.load() <= 0 || clock_settings.time_black.load() <= 0)
            {
                clock_settings.active.store(false);
                break;
            }

            previous_iteration = clock_settings.player_turn.load();
        }
        Board::setTime(clock_settings.time_white.load(), clock_settings.time_black.load());
        i++;
        std::this_thread::sleep_until(iteration_start + interval);
    }
}