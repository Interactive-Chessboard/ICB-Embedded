// game_clock.cpp
#include "game_clock.hpp"

std::string formatTimeMs(int ms)
{
    if (ms <= 0)
        return "0.0";

    int total_ds = ms / 100;
    int ds = total_ds % 10;
    int total_s = total_ds / 10;

    int s = total_s % 60;
    int total_m = total_s / 60;

    int m = total_m % 60;
    int h = total_m / 60;

    std::ostringstream out;
    if (h > 0)
        out << h << ":" << std::setw(2) << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0') << s << "." << ds;
    else if (m > 0)
        out << m << ":" << std::setw(2) << std::setfill('0') << s << "." << ds;
    else
        out << s << "." << ds;

    return out.str();
}



void tick(ClockSetting& clock_settings, Color previous_iteration)
{
    if (!clock_settings.active.load()) return;

    // Decrement timer
    if (clock_settings.player_turn.load() == Color::White)
    {
        clock_settings.time_white.fetch_sub(1);
    }
    else if (clock_settings.player_turn.load() == Color::Black)
    {
        clock_settings.time_black.fetch_sub(1);
    }
    else
    {
        clock_settings.active.store(false);
    }

    // Add extra time if changed turn
    if (clock_settings.player_turn.load() == Color::White &&
        previous_iteration != clock_settings.player_turn.load())
    {
        clock_settings.time_black.fetch_add(clock_settings.extra_time.load());
    }
    else if (clock_settings.player_turn.load() == Color::Black &&
        previous_iteration != clock_settings.player_turn.load())
    {
        clock_settings.time_white.fetch_add(clock_settings.extra_time.load());
    }

    // Verify clocks reached 0
    if (clock_settings.time_white.load() <= 0 || clock_settings.time_black.load() <= 0)
    {
        clock_settings.active.store(false);
    }

}


void startGameClock(ClockSetting &clock_settings, std::atomic<bool> &stop_clock_loop)
{
    using clock = std::chrono::steady_clock;
    constexpr std::chrono::milliseconds interval(1);
    auto start = clock::now();
    int i = 0;
    Color previous_iteration = Color::White;
    while(!stop_clock_loop.load())
    {
        auto iteration_start = start + i * interval;
        i++;
        {
        std::lock_guard<std::mutex> lock(clock_settings.mtx);

        tick(clock_settings, previous_iteration);
        previous_iteration = clock_settings.player_turn.load();
        }
        if ((clock_settings.time_white.load() % 10 == 0 && clock_settings.player_turn.load() == Color::White) ||
            (clock_settings.time_black.load() % 10 == 0 && clock_settings.player_turn.load() == Color::Black))
        {
            std::vector<std::string> time_display_msg = {
                "White Time: " + formatTimeMs(clock_settings.time_white.load()),
                "Black Time: " + formatTimeMs(clock_settings.time_black.load())
            };
            Hardware::get().setTimeScreen(time_display_msg);
        }
        std::this_thread::sleep_until(iteration_start + interval);
    }
}
