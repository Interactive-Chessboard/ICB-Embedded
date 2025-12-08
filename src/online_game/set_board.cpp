// set_board.cpp
#include "set_board.hpp"


void setClockSettings(ClockSetting &clock_settings, const std::string& request)
{
    std::string clock_settings = extract_value(request, "clock");

}


std::string setBoard(ClockSetting &clock_settings, const std::string& request, std::atomic<bool>& end_task_flag)
{
    setClockSettings(std::ref(clock_settings), request);

    while (!end_task_flag.load())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}