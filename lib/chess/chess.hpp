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


enum class Color {
    White,
    Black,
    Nil
};

enum class PieceType {
    Pawn,
    Rook,
    Knight,
    Bishop,
    Queen,
    King,
    Nil
};


enum class Winner {
    White,
    Black,
    Draw,
    Nil
};


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
    std::array<Piece, 64> board{
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
    std::array<char, 4> castle{'K', 'Q', 'k', 'q'};
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