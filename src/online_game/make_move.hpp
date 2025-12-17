// make_move.hpp
#pragma once
#include <atomic>
#include <string>
#include <stdexcept>
#include <vector>
#include "game_clock/game_clock.hpp"
#include "extract_value.hpp"
#include "chess.hpp"
#include "set_board.hpp" //temp

class MakeMove
{
private:
    ChessGame game;
    std::vector<Move> moves;
    std::vector<int> lifted;
    std::vector<int> placed;

    LedColor old_move_color = (0, 0, 255);
    LedColor lifted_square_color = (0, 230, 0);
    LedColor legal_moves_color = (0, 255, 0);
    LedColor illegal_moves_color = (255, 0, 0);

    int past_move_from;
    int past_move_to;

    uint64_t original_bit_board;
    uint64_t current_bit_board;

    void construct();
    uint64_t getGameBitBoard(ChessGame);

public:
    // Full constructor
    MakeMove(ChessGame game,
             LedColor old_move_color,
             LedColor lifted_square_color,
             LedColor legal_moves_color,
             LedColor illegal_moves_color,
             int past_move_from,
             int past_move_to)
        : game(std::move(game)),
          lifted(),
          placed(),
          old_move_color(old_move_color),
          lifted_square_color(lifted_square_color),
          legal_moves_color(legal_moves_color),
          illegal_moves_color(illegal_moves_color),
          past_move_from(past_move_from),
          past_move_to(past_move_to)
    {
        construct();
    }

    // Delegating constructor
    MakeMove(ChessGame game,
             int past_move_from,
             int past_move_to)
        : MakeMove(std::move(game),
                   old_move_color,
                   lifted_square_color,
                   legal_moves_color,
                   illegal_moves_color,
                   past_move_from,
                   past_move_to)
    {
        construct();
    }
};


struct BitChange {
    int index;   // Bit index [0–63]
    bool to;     // New bit value (after the change)
};


std::string makeMove(ClockSetting &clock_settings, const std::string& request, std::atomic<bool>& end_task_flag);