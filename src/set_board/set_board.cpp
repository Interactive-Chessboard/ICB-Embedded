// set_board.cpp
#include "set_board.hpp"


std::array<LedColor, 64> SetBoard::lightUpDifference(uint64_t current, uint64_t desired, LedColor color)
{
    std::array<LedColor, 64> result{};
    uint64_t diff = current ^ desired;

    for (int i = 0; i < 64; i++)
    {
        int bit = 63 - i;
        result[i] = (diff & (1ULL << bit)) ? color : LedColor();
    }

    return result;
}


std::string SetBoard::startOnline(const std::atomic<bool>& end_task_flag)
{
    while (!end_task_flag.load() && timeout > 0)
    {
        uint64_t current_board = Hardware::get().getBoardArr();
        if (current_board == board)
        {
            return "Ok";
        }

        std::array<LedColor, 64> lights = lightUpDifference(current_board, board, color);
        Hardware::get().setLed(lights);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        timeout--;
    }
    return "Error, timeout reached or end task called";
}


void SetBoard::startOffline(const std::atomic<bool>& active)
{
    while (active.load())
    {
        uint64_t current_board = Hardware::get().getBoardArr();
        if (current_board == board)
            return;

        std::array<LedColor, 64> lights = lightUpDifference(current_board, board, color);
        Hardware::get().setLed(lights);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
