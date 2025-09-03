#pragma once
#include "chess_game_struct.hpp"
struct Move
{
    int from_square;
    int to_square;
    ChessGame chess_game;
};