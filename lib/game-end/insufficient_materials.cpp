// insufficient_materials.cpp
#include "game_end.hpp"

// Insufficient materials is only king, knight + king, bishop + king or knight + bishop + king
bool GameEnd::insufficientMaterials(const ChessGame &chess_game, Color color)
{
    int bishop_count;
    int knight_count;
    for (int i = 0; i < 64; i++)
    {
        if (chess_game.board[i] == Piece(color, PieceType::Queen))
        {
            return false;
        }
        if (chess_game.board[i] == Piece(color, PieceType::Rook))
        {
            return false;
        }
        if (chess_game.board[i] == Piece(color, PieceType::Pawn))
        {
            return false;
        }
        if (chess_game.board[i] == Piece(color, PieceType::Knight))
        {
            knight_count++;
        }
        if (chess_game.board[i] == Piece(color, PieceType::Bishop))
        {
            knight_count++;
        }
    }
    if (knight_count >= 2 || bishop_count >= 2)
    {
        return false;
    }
    return true;
}