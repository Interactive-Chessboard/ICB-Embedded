// game_end.hpp
#pragma once
#include <vector>
#include "chess_game_struct.hpp"
#include "move_struct.hpp"
namespace GameEnd
{
void gameEndAnimation(char);
char calculateEndGame(ChessGame, std::vector<Move>);
}