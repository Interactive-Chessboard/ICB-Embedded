// structs.hpp
#pragma once
#include "enums.hpp"
#include <mutex>
#include <atomic>
#include <array>

struct Piece {
    Color color;
    PieceType piece_type;
    Piece(Color color = Color::Nil, PieceType piece_type = PieceType::Nil) 
        : color(color), piece_type(piece_type) {}
    bool operator==(const Piece& other) const {
        return color == other.color && piece_type == other.piece_type;
    }
    bool operator!=(const Piece& other) const {
        return color != other.color || piece_type != other.piece_type;
    }
};

struct ChessGame
{
    Color player_turn{Color::White};
    Piece board[64]{
        // Rank 1 (White back rank)
        Piece(Color::White, PieceType::Rook),
        Piece(Color::White, PieceType::Knight),
        Piece(Color::White, PieceType::Bishop),
        Piece(Color::White, PieceType::Queen),
        Piece(Color::White, PieceType::King),
        Piece(Color::White, PieceType::Bishop),
        Piece(Color::White, PieceType::Knight),
        Piece(Color::White, PieceType::Rook),

        // Rank 2 White pawns)
        Piece(Color::White, PieceType::Pawn), Piece(Color::White, PieceType::Pawn),
        Piece(Color::White, PieceType::Pawn), Piece(Color::White, PieceType::Pawn),
        Piece(Color::White, PieceType::Pawn), Piece(Color::White, PieceType::Pawn),
        Piece(Color::White, PieceType::Pawn), Piece(Color::White, PieceType::Pawn),

        // Rank 3 to 6
        Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(),
        Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(),
        Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(),
        Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(), Piece(),

        // Rank 7 (Black pawns)
        Piece(Color::Black, PieceType::Pawn), Piece(Color::Black, PieceType::Pawn),
        Piece(Color::Black, PieceType::Pawn), Piece(Color::Black, PieceType::Pawn),
        Piece(Color::Black, PieceType::Pawn), Piece(Color::Black, PieceType::Pawn),
        Piece(Color::Black, PieceType::Pawn), Piece(Color::Black, PieceType::Pawn),

        // Rank 8 (Black back rank)
        Piece(Color::Black, PieceType::Rook),
        Piece(Color::Black, PieceType::Knight),
        Piece(Color::Black, PieceType::Bishop),
        Piece(Color::Black, PieceType::Queen),
        Piece(Color::Black, PieceType::King),
        Piece(Color::Black, PieceType::Bishop),
        Piece(Color::Black, PieceType::Knight),
        Piece(Color::Black, PieceType::Rook),
    };
    char castle[4]{'K', 'Q', 'k', 'q'};
    int en_passant {-1};
    Winner winner = Winner::Nil;
};


struct Move
{
    int from_square;
    int to_square;
    Piece promotion;
    ChessGame chess_game;

    Move(int from = -1, int to = -1, Piece promo = Piece(), ChessGame cg = ChessGame())
        : from_square(from), to_square(to), promotion(promo), chess_game(cg) {}
};


