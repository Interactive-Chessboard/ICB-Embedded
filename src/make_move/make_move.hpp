// make_move.hpp
#pragma once
#include <atomic>
#include <string>
#include <stdexcept>
#include <vector>
#include <utility>
#include <unordered_set>
#include "chess.hpp"
#include "hardware.hpp"
#include "screen_selection/screen_selection.hpp"


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


class MakeMove
{
private:
    ChessGame game;
    std::vector<Move> moves;
    int lifted = -1;
    int lifted_opponent = -1;
    int lifted_special = -1;
    int placed = -1;
    int placed_special = -1;
    std::unordered_set<int> illegal_lifted;
    std::unordered_set<int> illegal_placed;

    LedColor past_move_color = LedColor(0, 0, 255);
    LedColor lifted_square_color = LedColor(0, 230, 0);
    LedColor legal_moves_color = LedColor(0, 255, 0);
    LedColor illegal_moves_color = LedColor(255, 0, 0);

    int past_move_from;
    int past_move_to;

    uint64_t original_bit_board;
    uint64_t current_bit_board;

    int timeout;

    void construct();
    std::pair<bool, bool> determineSpecialMoveLift(Move, int);
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
        moves = Chess::generateLegalMoves(game);
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
    {}

    std::string startOnline(const std::atomic<bool>&);
    Move startOffline(const std::atomic<bool>&);
};
