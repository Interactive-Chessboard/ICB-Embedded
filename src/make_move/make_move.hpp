// make_move.hpp
#pragma once
#include <atomic>
#include <string>
#include <stdexcept>
#include <vector>
#include <utility>
#include <unordered_set>
#include "chess.hpp"
#include "board.hpp"


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

    LedColor past_move_color = (0, 0, 255);
    LedColor lifted_square_color = (0, 230, 0);
    LedColor legal_moves_color = (0, 255, 0);
    LedColor illegal_moves_color = (255, 0, 0);

    int past_move_from;
    int past_move_to;

    uint64_t original_bit_board;
    uint64_t current_bit_board;

    bool use_timeout;
    int time_out;

    void construct();
    uint64_t getGameBitBoard(ChessGame);
    std::pair<bool, bool> determineSpecialMoveLift(Move, int);
    bool detectChangeTick(uint64_t);
    int calculateMoveTick();
    std::array<LedColor, 64> getBoardLights();

public:
    // Online game constructor
    MakeMove(ChessGame game,
             int past_move_from,
             int past_move_to,
             LedColor past_move_color,
             LedColor lifted_square_color,
             LedColor legal_moves_color,
             LedColor illegal_moves_color
             )
        : game(std::move(game)),
          past_move_from(past_move_from),
          past_move_to(past_move_to),
          past_move_color(past_move_color),
          lifted_square_color(lifted_square_color),
          legal_moves_color(legal_moves_color),
          illegal_moves_color(illegal_moves_color)
    {
        construct();
    }

    // Offline game constructor
    MakeMove(ChessGame game,
             int past_move_from,
             int past_move_to)
        : MakeMove(std::move(game),
                   past_move_from,
                   past_move_to,
                   past_move_color,
                   lifted_square_color,
                   legal_moves_color,
                   illegal_moves_color)
    {
        construct();
    }

    Move startOnline(std::atomic<bool>&, int);
    Move startOffline();
};
