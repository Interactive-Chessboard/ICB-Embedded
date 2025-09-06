#pragma once
#include "enums.hpp"

struct ChessGame
{
    Color player_turn{Color::White};
    Piece board[64]{
        // Rank 1 (White back rank)
        Piece{Color::White, PieceType::Rook},
        Piece{Color::White, PieceType::Knight},
        Piece{Color::White, PieceType::Bishop},
        Piece{Color::White, PieceType::Queen},
        Piece{Color::White, PieceType::King},
        Piece{Color::White, PieceType::Bishop},
        Piece{Color::White, PieceType::Knight},
        Piece{Color::White, PieceType::Rook},

        // Rank 2 (White pawns)
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn},
        Piece{Color::White, PieceType::Pawn}, Piece{Color::White, PieceType::Pawn},

        // Rank 3
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},

        // Rank 4
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},

        // Rank 5
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},

        // Rank 6
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},
        Piece{Color::Nil, PieceType::Nil}, Piece{Color::Nil, PieceType::Nil},

        // Rank 7 (Black pawns)
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},
        Piece{Color::Black, PieceType::Pawn}, Piece{Color::Black, PieceType::Pawn},

        // Rank 8 (Black back rank)
        Piece{Color::Black, PieceType::Rook},
        Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Bishop},
        Piece{Color::Black, PieceType::Queen},
        Piece{Color::Black, PieceType::King},
        Piece{Color::Black, PieceType::Bishop},
        Piece{Color::Black, PieceType::Knight},
        Piece{Color::Black, PieceType::Rook},
    };
    char castle[4]{'K', 'Q', 'k', 'q'};
    int enPassant {-1};
};



#include <mutex>
#include <atomic>
struct ClockSetting
{
    std::atomic<Color> player_turn{Color::White}; // Players turn do determine the active clock
    std::atomic<Winner> winner{Winner::Nil}; // Winner declared if a player has lost on time
    std::atomic<int> time_white; // Time remaining on whites clock in 1/100th of a second
    std::atomic<int> time_black; // Same for black
    std::atomic<int> extra_time; // Extra time in 1/100th of a second to add after each move
    std::atomic<bool> white_start_timer{false}; // Start the timer after whites first move
    std::atomic<bool> black_start_timer{false}; // Same for black
    std::atomic<bool> active{true}; // True if a winner hasn't been declared
    std::mutex mtx; // Mutex to synchronize
    ClockSetting(int game_time, int extra_time)
                : time_white(game_time), time_black(game_time), extra_time(extra_time) {}
};


struct Move
{
    int from_square;
    int to_square;
};

struct Settings
{
  GameMode game_mode;
  Color player_color;
  int game_time_min; // Game time in minutes
  int extra_time_sec; // Extra time per move in seconds
  int bot_strenght; // Bots strenght in elo
};