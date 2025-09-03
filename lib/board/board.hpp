// board.hpp
#pragma once
#include <vector>
#include "clock_struct.hpp"
#include "move_struct.hpp"
namespace Board
{
bool waitForInitBoard(int);
void playerTurnDetectMoves(ClockSetting&, std::vector<Move>);
void playerMakeOpponentMove(ClockSetting&, Move);
};