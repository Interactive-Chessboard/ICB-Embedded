// legal_moves.hpp
#pragma once
#include <vector>
#include "structs.hpp"
namespace LegalMoves
{
std::vector<Move> generateLegalMoves(ChessGame);
ChessGame updateGame(ChessGame, Move);
}