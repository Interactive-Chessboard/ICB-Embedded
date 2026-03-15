// set_board.hpp
#pragma once
#include <array>
#include <string>
#include <atomic>
#include <chrono>
#include <thread>
#include "hardware.hpp"


/**
 * @struct ExtractSetBoard
 * @brief Bundle of data used to start a SetBoard session.
 *
 * Contains the target board bitboard, LED color for highlighting differences,
 * and a timeout value. Passed to SetBoard to configure online board setup.
 */
struct ExtractSetBoard
{
    LedColor color;
    uint64_t board;
    int timeout;
};


/**
 * @class SetBoard
 * @brief Guides the user to match the physical board to a target position.
 *
 * Continuously compares the current hardware board state to a desired bitboard.
 * Squares that differ are highlighted with LEDs so the user can place or remove
 * pieces accordingly. Completes once the board matches the target position.
 *
 * Supports online mode (with timeout and status string) and offline mode
 * (runs until complete or cancelled).
 */
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
