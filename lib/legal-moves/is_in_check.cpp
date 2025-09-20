// is_in_check.cpp
#include "legal_moves.hpp"

bool LegalMoves::isInCheck(ChessGame chess_game, int square)
{
    // Find the king
    if (square == -1)
    {
        for (int i = 0; i < 64; i++)
        {
            if (chess_game.board[i] == Piece(chess_game.player_turn, PieceType::King))
            {
                square = i;
                break;
            }
        }
    }
    if (square == -1)
    {
        return false;
    }

    // Look for black pawns attacking a white king
    if (chess_game.player_turn == Color::White)
    {
        if ((square + 7) % 8 != 7 && square + 7 < 64 &&
            chess_game.board[square + 7] == Piece(Color::Black, PieceType::Pawn))
        {
            return true;
        }
        if ((square + 9) % 8 != 0 && square + 9 < 64 &&
            chess_game.board[square + 9] == Piece(Color::Black, PieceType::Pawn))
        {
            return true;
        }
    }

    // Look for white pawns attacking a black king
    if (chess_game.player_turn == Color::Black)
    {
        if ((square - 9) % 8 != 7 && square - 9 >= 0 &&
            chess_game.board[square - 9] == Piece(Color::White, PieceType::Pawn))
        {
            return true;
        }
        if ((square - 7) % 8 != 0 && square - 7 >= 0 &&
            chess_game.board[square - 7] == Piece(Color::White, PieceType::Pawn))
        {
            return true;
        }
    }

    Color opponent_color = chess_game.player_turn == Color::White ? Color::Black : Color::White;

    // Knights
    int knight_moves_left[4] = {square - 17, square - 10, square + 6, square + 15}; // kight moves (not magic numbers)
    int knight_moves_right[4] = {square - 15, square - 6, square + 10, square + 17};
    for (int i = 0; i < 4; i++)
    {
        if (knight_moves_left[i] < 64 && knight_moves_left[i] >= 0 && square % 8 > knight_moves_left[i] % 8 && 
            chess_game.board[knight_moves_left[i]] == Piece(opponent_color, PieceType::Knight))
        {
            return true;
        }
        if (knight_moves_right[i] < 64 && knight_moves_right[i] >= 0 && square % 8 < knight_moves_right[i] % 8 && 
            chess_game.board[knight_moves_right[i]] == Piece(opponent_color, PieceType::Knight))
        {
            return true;
        }
    }

    // Bishop and Queen
    // bottom left
    int bottom_left = square - 9;
    while (bottom_left >= 0 && bottom_left % 8 != 7)
    {
        if (chess_game.board[bottom_left] == Piece(opponent_color, PieceType::Bishop) ||
                chess_game.board[bottom_left] == Piece(opponent_color, PieceType::Queen))
        {
            return true;
        }
        else if (chess_game.board[bottom_left] != Piece())
        {
            break;
        }
        bottom_left -= 9;
    }

    // bottom right
    int bottom_right = square - 7;
    while (bottom_right >= 0 && bottom_right % 8 != 0)
    {
        if (chess_game.board[bottom_right] == Piece(opponent_color, PieceType::Bishop) ||
                chess_game.board[bottom_right] == Piece(opponent_color, PieceType::Queen))
        {
            return true;
        }
        else if (chess_game.board[bottom_right] != Piece())
        {
            break;
        }
        bottom_right -= 7;
    }

    // up left
    int up_left = square + 7;
    while (up_left < 64 && up_left % 8 != 7)
    {
        if (chess_game.board[up_left] == Piece(opponent_color, PieceType::Bishop) ||
                chess_game.board[up_left] == Piece(opponent_color, PieceType::Queen))
        {
            return true;
        }
        else if (chess_game.board[up_left] != Piece())
        {
            break;
        }
        up_left += 7;
    }

    // up right
    int up_right = square + 9;
    while (up_right < 64 && up_right % 8 != 0)
    {
        if (chess_game.board[up_right] == Piece(opponent_color, PieceType::Bishop) ||
                chess_game.board[up_right] == Piece(opponent_color, PieceType::Queen))
        {
            return true;
        }
        else if (chess_game.board[up_right] != Piece())
        {
            break;
        }
        up_right += 9;
    } 


    // Rook and Queen
    // left
    int left = square - 1;
    while (left >= 0 && left % 8 != 7)
    {
        if (chess_game.board[left] == Piece(opponent_color, PieceType::Rook) ||
                chess_game.board[left] == Piece(opponent_color, PieceType::Queen))
        {
            return true;
        }
        else if (chess_game.board[left] != Piece())
        {
            break;
        }
        left--;
    }

    // right
    int right = square + 1;
    while (right < 64 && right % 8 != 0)
    {
        if (chess_game.board[right] == Piece(opponent_color, PieceType::Rook) ||
                chess_game.board[right] == Piece(opponent_color, PieceType::Queen))
        {
            return true;
        }
        else if (chess_game.board[right] != Piece())
        {
            break;
        }
        right++;
    }

    // bottom
    int bottom = square - 8;
    while (bottom >= 0)
    {
        if (chess_game.board[bottom] == Piece(opponent_color, PieceType::Rook) ||
                chess_game.board[bottom] == Piece(opponent_color, PieceType::Queen))
        {
            return true;
        }
        else if (chess_game.board[bottom] != Piece())
        {
            break;
        }
        bottom -= 8;
    }

    // up
    int up = square + 8;
    while (up < 64)
    {
        if (chess_game.board[up] == Piece(opponent_color, PieceType::Rook) ||
                chess_game.board[up] == Piece(opponent_color, PieceType::Queen))
        {
            return true;
        }
        else if (chess_game.board[up] != Piece())
        {
            break;
        }
        up += 8;
    }


    // King
    int king_moves_left[3] = {square - 9, square - 1, square + 7}; // king moves (not magic numbers)
    int king_moves_right[3] = {square - 7, square + 1, square + 9};
    int king_moves[2] = {square - 8, square + 8};
    for (int i = 0; i < 3; i++)
    {
        if (king_moves_left[i] < 64 && king_moves_left[i] >= 0 && square % 8 > king_moves_left[i] % 8 && 
            chess_game.board[king_moves_left[i]] == Piece(opponent_color, PieceType::King))
        {
            return true;
        }
        if (king_moves_right[i] < 64 && king_moves_right[i] >= 0 && square % 8 < king_moves_right[i] % 8 && 
            chess_game.board[king_moves_right[i]] == Piece(opponent_color, PieceType::King))
        {
            return true;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if (king_moves[i] < 64 && king_moves[i] >= 0 && 
            chess_game.board[king_moves[i]] == Piece(opponent_color, PieceType::King))
        {
            return true;
        }
    }

    return false;
}