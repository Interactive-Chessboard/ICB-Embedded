// set_board.hpp
#pragma once
#include <array>
#include <string>
#include "board.hpp"


class SetBoard
{
private:
    LedColor color = (0, 0, 255);
    uint64_t board;
    std::array<LedColor, 64> lightUpDifference(uint64_t, uint64_t, LedColor);
public:
    SetBoard(LedColor color, uint64_t board)
        : color(color), board(board) {}

    SetBoard(uint64_t board)
        : board(board) {}

    std::string startOnline(std::atomic<bool>&, int);
    void startOffline(); // TO DO
};
