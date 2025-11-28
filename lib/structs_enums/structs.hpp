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


struct ClockSetting
{
    std::atomic<bool> active{true}; // True if a winner hasn't been declared
    std::atomic<Color> player_turn{Color::White}; // Players turn do determine the active clock
    std::atomic<int> time_white; // Time remaining on whites clock in 1/100th of a second
    std::atomic<int> time_black; // Same for black
    std::atomic<int> extra_time; // Extra time in 1/100th of a second to add after each move
    std::atomic<bool> white_start_timer{false}; // Start the timer after whites first move
    std::atomic<bool> black_start_timer{false}; // Same for black
    std::mutex mtx; // Mutex to synchronize
    ClockSetting(int game_time, int extra_time)
                : time_white(game_time), time_black(game_time), extra_time(extra_time) {}
};

struct Settings
{
  GameMode game_mode;
  Color player_color;
  int game_time_min; // Game time in minutes
  int extra_time_sec; // Extra time per move in seconds
  int bot_strenght; // Bots strenght in elo
};


struct FullChessGame
{
    ChessGame chessgame;
    std::array<u_int8_t, 3> old_color{0, 0, 0};
    int old_move_from;
    int old_move_to;
    std::array<u_int8_t, 3> lifted_color{0, 0, 0};
    std::array<u_int8_t, 3> legal_move_color{0, 0, 0};
    std::array<u_int8_t, 3> illegal_move_color{0, 0, 0};
    int white_clock;
    int black_clock;
    int extra_time;
    Color clock_run_down;
};
