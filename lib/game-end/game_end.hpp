// game_end.hpp
#pragma once
#include <vector>
#include "structs.hpp"
#include "enums.hpp"
namespace GameEnd
{
Winner calculateEndGame(ChessGame, std::vector<Move>);
Winner getClockWinner(const ClockSetting&, const ChessGame&);
bool insufficientMaterials(const ChessGame&, Color);
}