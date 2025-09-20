// legal_moves.hpp
#pragma once
#include <vector>
#include "structs.hpp"

namespace LegalMoves
{
std::vector<Move> generateLegalMoves(ChessGame);
bool isInCheck(ChessGame, int = -1);
}