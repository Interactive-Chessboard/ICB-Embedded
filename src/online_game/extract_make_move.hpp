// extract_make_move.hpp
#pragma once
#include "chess.hpp"
#include "hardware.hpp"
#include "extract_value.hpp"


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

ExtractMakeMove extractMakeMove(const std::string& request);
