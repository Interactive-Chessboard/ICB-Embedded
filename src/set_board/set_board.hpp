// set_board.hpp
#pragma once
#include <array>
#include <string>
#include <atomic>
#include <chrono>
#include <thread>
#include "hardware.hpp"
#include "chess.hpp"


/**
 * @struct ExtractSetBoard
 * @brief Bundle of data used to start a SetBoard session.
 *
 * Contains the target board bitboard, LED color for highlighting differences,
 * and a timeout value. Passed to SetBoard to configure online board setup.
 */
struct ExtractSetBoard
{
    ChessGame game;
    LedColor past_move_color;
    int past_move_from;
    int past_move_to;
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
    ChessGame game;
    LedColor past_move_color = LedColor(0, 0, 255);
    int past_move_from;
    int past_move_to;
    int timeout;

    bool capture_lifted = false;

    std::array<LedColor, 64> lightUpDifference(uint64_t, uint64_t, LedColor);

public:
    SetBoard(ExtractSetBoard e) : game(e.game), past_move_color(e.past_move_color), past_move_from(e.past_move_from),
                                  past_move_to(e.past_move_to), timeout(e.timeout) {}
    SetBoard(ChessGame game, int from, int to) : game(game), past_move_from(from), past_move_to(to) {}

    std::string startOnline(const std::atomic<bool>&);
    void startOffline(const std::atomic<bool>&);
};
