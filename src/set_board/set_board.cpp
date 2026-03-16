// set_board.cpp
#include "set_board.hpp"


std::array<LedColor, 64> SetBoard::lightUpDifference(uint64_t current, uint64_t desired, LedColor color)
{
    std::array<LedColor, 64> result{};
    uint64_t diff = current ^ desired;

    for (int i = 0; i < 64; i++)
    {
        int bit = 63 - i;
        if (diff & (1ULL << bit))
        {
            result[i] = color;
            // If move to square is not the same, the lifted capture is set to true
            if (i == past_move_to)
                capture_lifted = true;
        }
        else
            result[i] = LedColor();
    }
    // Light up past move to in a capture scenario even if the bitboard matches
    if (!capture_lifted && past_move_to != -1)
        result[past_move_to] = color;

    return result;
}


std::string SetBoard::startOnline(const std::atomic<bool>& active)
{
    uint64_t result_board = Chess::getGameBitBoard(game);
    if (past_move_to == -1)
        capture_lifted = true;

    while (active.load() && timeout > 0)
    {
        uint64_t current_board = Hardware::get().getBoardArr();
        if (capture_lifted && current_board == result_board)
        {
            Hardware::get().clearLed();
            return "ok";
        }

        std::array<LedColor, 64> lights = lightUpDifference(current_board, result_board, past_move_color);
        Hardware::get().setLed(lights);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        timeout--;
    }
    Hardware::get().clearLed();
    return "error, timeout reached or end task called";
}


void SetBoard::startOffline(const std::atomic<bool>& active)
{
    uint64_t result_board = Chess::getGameBitBoard(game);
    if (past_move_to == -1)
        capture_lifted = true;

    while (active.load())
    {
        uint64_t current_board = Hardware::get().getBoardArr();
        if (capture_lifted && current_board == result_board)
        {
            Hardware::get().clearLed();
            return;
        }

        std::array<LedColor, 64> lights = lightUpDifference(current_board, result_board, past_move_color);
        Hardware::get().setLed(lights);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    Hardware::get().clearLed();
}
