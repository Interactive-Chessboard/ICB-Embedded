// set_board.cpp
#include "set_board.hpp"


void setClockSettings(ClockSetting &clock_settings, const std::string& request)
{
    std::string clock_settings_str = extract_value(request, "clock");

    std::string active_str = extract_value(clock_settings_str, "active");
    bool active;
    if (active_str == "true")
        active = true;
    else if (active_str == "false")
        active = false;
    else
        throw std::runtime_error("Error, expecting square bracket");

    int white, black, extra_time;
    try
    {
        white = stoi(extract_value(clock_settings_str, "white"));
        black = stoi(extract_value(clock_settings_str, "black"));
        extra_time = stoi(extract_value(clock_settings_str, "extra_time"));
    }
    catch(...)
    {
        throw std::runtime_error("Error, time values are not ints");
    }
    if (white < 0 || black < 0 || extra_time < 0) throw std::runtime_error("Error, time values can't be negative");
    


    std::string run_down = extract_value(clock_settings_str, "run_down");
    ClockColor clock_color;
    if (run_down == "w")
        clock_color = ClockColor::White;
    else if (run_down == "b")
        clock_color = ClockColor::Black;
    else
        throw std::runtime_error("Error, expecting square bracket");

    {
    std::lock_guard<std::mutex> lock(clock_settings.mtx);

    clock_settings.active.store(active);
    clock_settings.time_white.store(white * 100);
    clock_settings.time_black.store(black * 100);
    clock_settings.extra_time.store(extra_time * 100);
    clock_settings.player_turn.store(clock_color);
    }


}


std::string setBoard(ClockSetting &clock_settings, const std::string& request, std::atomic<bool>& end_task_flag)
{
    try
    {
        setClockSettings(std::ref(clock_settings), request);
    }
    catch (const std::runtime_error& e)
    {
        return e.what();
    }

    while (!end_task_flag.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}