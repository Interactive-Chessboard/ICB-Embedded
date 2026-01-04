// set_board.cpp
#include "set_board.hpp"


std::array<LedColor, 64> SetBoard::lightUpDifference(uint64_t current, uint64_t desired, LedColor color)
{
    std::array<LedColor, 64> result{};
    uint64_t diff = current ^ desired;

    for (int i = 0; i < 64; ++i)
    {
        if (diff & (1ULL << i)) {
            result[i] = color;
        } else {
            result[i] = LedColor();
        }
    }

    return result;
}


std::string SetBoard::startOnline(std::atomic<bool>& end_task_flag, int timeout)
{
    while (!end_task_flag.load() && timeout > 0)
    {
        uint64_t current_board = Board::getBoardArr();
        if (current_board == board)
        {
            return "ok";
        }

        std::array<LedColor, 64> lights = lightUpDifference(current_board, board, color);
        Board::setLed(lights);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        timeout--;
    }
    return "Error, timeout reached or end task called";
}


void SetBoard::startOffline(ClockSetting& clock_settings)
{
    while (clock_settings.active.load())
    {
        uint64_t current_board = Board::getBoardArr();
        if (current_board == board)
        {
            return;
        }

        std::array<LedColor, 64> lights = lightUpDifference(current_board, board, color);
        Board::setLed(lights);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}