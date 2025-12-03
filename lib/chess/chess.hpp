// chess.hpp
#pragma once
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <map>
#include <array>
#include <limits.h>
#include "structs.hpp"

namespace Chess
{
std::vector<Move> generateLegalMoves(ChessGame);
bool isInCheck(ChessGame, int = -1);
Winner calculateEndGame(ChessGame, std::vector<Move>);
//Winner getClockWinner(const ClockSetting&, const ChessGame&);
bool insufficientMaterials(const ChessGame&, Color);
//void chess_clock(ClockSetting&);
Move botMove(ChessGame, std::vector<Move>);
}