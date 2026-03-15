// make_move.hpp
#pragma once
#include <atomic>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <stdexcept>
#include <vector>
#include <utility>
#include <unordered_set>
#include "chess.hpp"
#include "hardware.hpp"
#include "screen_selection/screen_selection.hpp"


/**
 * @struct ExtractMakeMove
 * @brief Bundle of data used to start a MakeMove session.
 *
 * Contains the current game state, LED color settings for board feedback,
 * the previous move (for highlighting), and a timeout value. Passed to
 * MakeMove to configure online move detection.
 */
struct ExtractMakeMove
{
    ChessGame game;
    LedColor past_move_color;
    LedColor lifted_square_color;
    LedColor legal_moves_color;
    LedColor illegal_moves_color;
    int past_move_from;
    int past_move_to;
    int timeout;
};


/**
 * @class MakeMove
 * @brief Interprets physical board interactions to determine a legal chess move.
 *
 * This class monitors a hardware chessboard represented as a 64-bit bitboard and
 * translates piece lifts and placements into a validated move within a given
 * ChessGame state. It is designed for use with an electronic or sensor-based board
 * where piece presence is detected per square and LED feedback can guide the user.
 *
 * Core responsibilities:
 * - Tracks the current game position and all legal moves for the side to move.
 * - Observes real-time board changes (piece lifts/placements) from hardware.
 * - Distinguishes valid interactions from illegal ones.
 * - Supports special moves including castling, en passant, and promotion.
 * - Provides visual feedback via LEDs (legal targets, illegal actions, past move).
 * - Determines when a complete legal move has been performed on the board.
 *
 * Interaction model:
 * - A move is inferred from a sequence of square state changes rather than an
 *   explicit command.
 * - The class maintains internal state for lifted pieces, captured pieces,
 *   partially completed moves, and illegal actions.
 * - Only when the physical board matches the resulting position of a legal move
 *   is that move accepted.
 *
 * Modes of operation:
 * - Online mode: Returns a serialized description of the move and enforces a timeout.
 * - Offline mode: Returns the Move object directly and optionally handles promotion
 *   selection via an external screen interface.
 *
 * Visual feedback:
 * - Highlights the previous move.
 * - Shows the currently lifted piece.
 * - Displays legal destination squares.
 * - Marks illegal lifts or placements.
 * - Indicates near-completion states (e.g., capture sequences).
 *
 * Preconditions:
 * - The physical board state must initially match the provided game position.
 * - Legal moves must correspond to the current player to move.
 *
 * This class does not modify the game state itself; it only detects and returns
 * a move that can then be applied externally.
 */
class MakeMove
{
private:
    ChessGame game;
    std::vector<Move> moves;

    int lifted = -1;
    int lifted_opponent = -1;
    int placed = -1;
    int lifted_castle = -1;
    int placed_castle = -1;
    std::unordered_set<int> illegal_lifted;
    std::unordered_set<int> illegal_placed;

    std::unordered_set<int> valid_lifted;
    std::unordered_set<int> valid_lifted_opponent;

    LedColor past_move_color = LedColor(0, 0, 255);
    LedColor lifted_square_color = LedColor(255, 255, 0);
    LedColor legal_moves_color = LedColor(0, 255, 0);
    LedColor illegal_moves_color = LedColor(255, 0, 0);

    int past_move_from;
    int past_move_to;

    uint64_t original_bit_board;
    uint64_t current_bit_board;

    int timeout;

    void initialize();
    std::pair<int, int> determineCastle(Move, int);
    bool detectChangeTick(uint64_t);
    int calculateMoveTick();
    std::array<LedColor, 64> getBoardLights();
    Move returnMove(const std::atomic<bool>&, int);

public:
    // Online game constructor
    MakeMove(ExtractMakeMove e)
        : game(e.game),
          past_move_from(e.past_move_from),
          past_move_to(e.past_move_to),
          past_move_color(e.past_move_color),
          lifted_square_color(e.lifted_square_color),
          legal_moves_color(e.legal_moves_color),
          illegal_moves_color(e.illegal_moves_color),
          timeout(e.timeout)
    {
        initialize();
    }

    // Offline game constructor
    MakeMove(ChessGame game,
             std::vector<Move> moves,
             int past_move_from,
             int past_move_to)
        : game(game),
          moves(moves),
          past_move_from(past_move_from),
          past_move_to(past_move_to)
    {
        initialize();
    }

    std::string startOnline(const std::atomic<bool>&);
    Move startOffline(const std::atomic<bool>&);
};
