// insufficient_materials.cpp
#include "game_end.hpp"

// Insufficient materials is only king, knight + king, bishop + king or knight + bishop + king
bool GameEnd::insufficientMaterials(const ChessGame &chess_game, Color color)
{
    int bishop_count = 0;
    int knight_count = 0;
    for (int i = 0; i < 64; i++)
    {
        Piece piece = chess_game.board[i];
        if (piece.color != color) continue;

        switch (piece.piece_type)
        {
            case PieceType::Queen:
            case PieceType::Rook:
            case PieceType::Pawn:
                return false;
            case PieceType::Knight:
                knight_count++;
                break;
            case PieceType::Bishop:
                bishop_count++;
                break;
            default:
                break;
        }
    }
    if (knight_count >= 2 || bishop_count >= 2)
    {
        return false;
    }
    return true;
}