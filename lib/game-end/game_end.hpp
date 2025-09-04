// game_end.hpp
#pragma once
#include <vector>
#include "structs.hpp"
namespace GameEnd
{
void gameEndAnimation(char);
char calculateEndGame(ChessGame, std::vector<Move>);
}