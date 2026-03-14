// chess.hpp
#pragma once
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <map>
#include <array>
#include <limits.h>


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


enum class BotStrength {
    Easy,
    Medium,
    Hard,
    Impossible
};


enum class MoveType {
    Normal,
    En_Passant,
    Castle
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
    MoveType move_type;
    ChessGame chess_game;

    Move(int from = -1, int to = -1, Piece promo = Piece(), MoveType mt = MoveType::Normal, ChessGame cg = ChessGame())
        : from_square(from), to_square(to), promotion(promo), move_type(mt), chess_game(cg) {}
};


namespace Chess
{
Move botMove(ChessGame, std::vector<Move>, BotStrength); // TODO


/**
 * @brief Determines the game result based on the current position and legal moves.
 *
 * This function evaluates whether the chess game has reached an end state,
 * including checkmate, stalemate, or a draw due to insufficient material.
 *
 * The outcome is determined as follows:
 * - If the current player has no legal moves and is in check, the game is checkmate.
 *   The opponent is declared the winner.
 * - If the current player has no legal moves and is NOT in check, the game is
 *   stalemate and declared a draw.
 * - If both sides lack sufficient material to deliver checkmate, the game is
 *   declared a draw.
 * - Otherwise, the game is still ongoing.
 *
 * @param chess_game The current game state, including board position and player turn.
 * @param legal_moves_size Size of a list of all legal moves available to the current player.
 *
 * @return Winner The result of the game:
 * - Winner::White  — White has won (Black is checkmated)
 * - Winner::Black  — Black has won (White is checkmated)
 * - Winner::Draw   — Draw by stalemate or insufficient material
 * - Winner::Nil    — No winner yet; the game continues
 */
Winner calculateEndGame(const ChessGame&, int);


/**
 * @brief Determines the game result when a player's clock expires.
 *
 * This function evaluates the outcome of a chess game when one or both players
 * run out of time. According to chess rules, a player loses on time only if the
 * opponent has sufficient material remaining to deliver checkmate.
 *
 * The outcome is determined as follows:
 * - If White's time has expired and Black has sufficient mating material,
 *   Black is declared the winner.
 * - If Black's time has expired and White has sufficient mating material,
 *   White is declared the winner.
 * - If the opponent does NOT have sufficient material to checkmate,
 *   the game is declared a draw.
 * - If both clocks have expired (or neither side can win on time),
 *   the result is a draw.
 *
 * @param time_white Remaining time for White (in milliseconds or seconds,
 *                   depending on the clock implementation).
 * @param time_black Remaining time for Black (same units as time_white).
 * @param chess_game The current game state, used to determine material
 *                   sufficiency for checkmate.
 *
 * @return Winner The result of the game:
 * - Winner::White — White wins on time
 * - Winner::Black — Black wins on time
 * - Winner::Draw  — Draw due to insufficient mating material or both flags down
 */
Winner getClockWinner(int, int, const ChessGame&);


/**
 * @brief Generates a bitboard representing all occupied squares on the board.
 *
 * This function constructs a 64-bit bitboard where each bit corresponds to a
 * square on the chessboard. A bit is set to 1 if the square contains any piece
 * (regardless of color or type), and 0 if the square is empty.
 *
 * The mapping uses the board's linear index (0–63). Bit 63 corresponds to
 * index 0, and bit 0 corresponds to index 63, meaning higher-order bits
 * represent earlier squares in the board array.
 *
 * @param game The current game state containing the board representation.
 *
 * @return uint64_t A 64-bit bitboard with bits set for all occupied squares.
 *
 * @note This bitboard encodes occupancy only; it does not distinguish between
 *       piece types or colors.
 */
uint64_t getGameBitBoard(const ChessGame&);


/**
 * @brief Determines whether a player has insufficient material to deliver checkmate.
 *
 * This function checks if the specified player (color) lacks the necessary
 * pieces to force checkmate, regardless of the opponent’s material.
 *
 * A player is considered to have sufficient mating material if they possess
 * any of the following:
 * - At least one pawn, rook, or queen
 * - Two or more bishops
 * - Two or more knights
 *
 * Otherwise, the material is treated as insufficient. The function therefore
 * considers the following as insufficient material:
 * - King alone
 * - King + single bishop
 * - King + single knight
 * - King + bishop + knight (cannot force mate without opponent cooperation)
 *
 * @param chess_game The current game state containing the board position.
 * @param color The player color whose material is being evaluated.
 *
 * @return true  If the specified player has insufficient material to checkmate.
 * @return false If the player has sufficient material to potentially deliver mate.
 *
 * @note This check evaluates only the material of the given player and does not
 *       consider the opponent’s pieces or board position.
 */
bool insufficientMaterials(const ChessGame&, Color);


/**
 * @brief Determines whether the current player's king (or a specified square)
 *        is under attack.
 *
 * This function checks if a square is attacked by any opponent piece. If no
 * square is specified (square == -1), the function first locates the king of
 * the player whose turn it is and evaluates whether that king is in check.
 *
 * The function tests attacks from all piece types according to standard chess
 * movement rules:
 * - Pawns (direction depends on player turn)
 * - Knights
 * - Bishops and queens (diagonals)
 * - Rooks and queens (ranks and files)
 * - Opposing king (adjacent squares)
 *
 * Sliding piece attacks (bishops, rooks, queens) are blocked by the first
 * encountered piece along each ray.
 *
 * @param chess_game The current game state, including board position and
 *                   player to move.
 * @param square The board index (0–63) to test for attacks. If -1, the function
 *               automatically finds the current player's king and tests that
 *               square instead.
 *
 * @return true  If the specified square (or the current player's king) is under attack.
 * @return false Otherwise.
 *
 * @note This function checks attacks relative to the current player's turn,
 *       meaning it evaluates threats from the opponent’s pieces.
 */
bool isInCheck(const ChessGame&, int = -1);


/**
 * @brief Generates all fully legal moves for the current player.
 *
 * This function computes every move that the player to move is allowed to
 * make according to the rules of chess. It filters out moves that would leave
 * the player's king in check.
 *
 * The process consists of several stages:
 * 1. Generate pseudo-legal moves (moves that follow piece movement rules
 *    but may leave the king in check).
 * 2. Add special moves, including en passant and castling, when available.
 * 3. Simulate each move and discard any that result in the current player's
 *    king being in check.
 * 4. For each legal move, update the resulting game state and switch the
 *    player turn.
 *
 * Special rules handled:
 * - Pawn promotions
 * - En passant captures
 * - Castling (including check restrictions on path squares)
 * - Self-check prevention (illegal positions are filtered out)
 *
 * @param chess_game The current game state, including board position,
 *                   player to move, castling rights, and en passant square.
 *
 * @return std::vector<Move> A list of all legal moves available to the
 *                           current player. Each Move contains the resulting
 *                           game state after the move is applied.
 *
 * @note This function assumes the input position is valid.
 * @note If no legal moves are returned, the position is either checkmate
 *       or stalemate (must be determined separately).
 */
std::vector<Move> generateLegalMoves(const ChessGame&);
}
