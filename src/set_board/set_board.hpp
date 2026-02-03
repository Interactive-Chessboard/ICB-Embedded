// set_board.hpp
#pragma once
#include <array>
#include <string>
#include <atomic>
#include <chrono>
#include <thread>
#include "hardware.hpp"


struct ExtractSetBoard
{
    LedColor color;
    uint64_t board;
    int timeout;
};


class SetBoard
{
private:
    LedColor color = LedColor(0, 0, 255);
    uint64_t board;
    std::array<LedColor, 64> lightUpDifference(uint64_t, uint64_t, LedColor);

    int timeout;
public:
    SetBoard(ExtractSetBoard e) : color(e.color), board(e.board), timeout(e.timeout) {}
    SetBoard(uint64_t board) : board(board) {}

    std::string startOnline(const std::atomic<bool>&);
    void startOffline(const std::atomic<bool>&);
};
