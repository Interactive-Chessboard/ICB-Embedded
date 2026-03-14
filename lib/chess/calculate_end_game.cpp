// calculate_end_game.cpp
#include "chess.hpp"

Winner Chess::calculateEndGame(const ChessGame &chess_game, int legal_moves_size)
{
    bool in_check = Chess::isInCheck(chess_game);
    if (legal_moves_size <= 0 && in_check)
    {
        return (chess_game.player_turn == Color::White) ? Winner::Black : Winner::White;
    }
    else if (legal_moves_size <= 0)
    {
        return Winner::Draw;
    }
    if (Chess::insufficientMaterials(chess_game, Color::White) &&
        Chess::insufficientMaterials(chess_game, Color::Black))
    {
        return Winner::Draw;
    }
    return Winner::Nil;
}
