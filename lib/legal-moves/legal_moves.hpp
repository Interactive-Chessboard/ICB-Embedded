// legal_moves.hpp
#pragma once
#include <vector>
#include "move_struct.hpp"
#include "chess_game_struct.hpp"
namespace LegalMoves
{
std::vector<Move> generateLegalMoves(ChessGame);
}