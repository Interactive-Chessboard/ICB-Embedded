// board.hpp
#pragma once
#include <vector>
#include "structs.hpp"
namespace Board
{
bool waitForInitBoard(int);
void playerTurnDetectMoves(ClockSetting&, std::vector<Move>);
void playerMakeOpponentMove(ClockSetting&, Move);
};