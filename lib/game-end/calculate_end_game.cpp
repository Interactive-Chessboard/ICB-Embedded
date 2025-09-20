// calculate_end_game.cpp
#include "game_end.hpp"
#include "legal_moves.hpp"

Winner GameEnd::calculateEndGame(ChessGame chess_game, std::vector<Move> legal_moves)
{
    bool in_check = LegalMoves::isInCheck(chess_game);
    if (legal_moves.size() <= 0 && in_check)
    {
        return (chess_game.player_turn == Color::White) ? Winner::Black : Winner::White;
    }
    else if (legal_moves.size() <= 0)
    {
        return Winner::Draw;
    }
    if (GameEnd::insufficientMaterials(chess_game, Color::White) && 
        GameEnd::insufficientMaterials(chess_game, Color::Black))
    {
        return Winner::Draw;
    }
    return Winner::Nil;
}