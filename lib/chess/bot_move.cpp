// bot_move.cpp
#include "chess.hpp"

const std::map<PieceType, int> piece_value = 
{
    {PieceType::Nil, 0},
    {PieceType::Pawn, 100},
    {PieceType::Knight, 290},
    {PieceType::Bishop, 310},
    {PieceType::Rook, 500},
    {PieceType::Queen, 900},
    {PieceType::King, 60000}
};

const std::map<PieceType, std::array<int, 64>> square_value = 
{
    {PieceType::Nil, std::array<int, 64>{}},
    
    {PieceType::Pawn, {
        0, 0, 0, 0, 0, 0, 0, 0,
        -30, 9, -6, -35, -36, -13, 4, -29,
        -21, 8, 6, -12, -9, -1, 3, -21,
        -28, 3, 11, 10, 6, 1, 1, -23,
        -19, 17, -2, 14, 13, 0, 13, -12,
        5, 28, 21, 46, 40, 29, 42, 8,
        79, 83, 86, 71, 100, 83, 88, 88,
        0, 0, 0, 0, 0, 0, 0, 0
    }},

    {PieceType::Knight, {
        -75, -22, -28, -24, -20, -35, -23, -68,
        -22, -17, 2, 0, 2, 0, -24, -19,
        -19, 11, 14, 22, 17, 15, 10, -15,
        -1, 4, 32, 19, 22, 35, 1, 0,
        26, 25, 46, 38, 33, 39, 26, 16,
        9, 65, 0, 76, 75, 28, 62, -3,
        -3, -7, 101, -35, 3, 61, -6, -13,
        -67, -51, -73, -74, -12, -56, -58, -71
    }},

    {PieceType::Bishop, {
        -8, 3, -14, -13, -13, -16, -10, -10,
        19, 21, 10, 6, 6, 6, 20, 17,
        15, 26, 23, 17, 7, 26, 20, 14,
        13, 10, 16, 22, 17, 16, 1, 8,
        24, 16, 21, 35, 27, 27, 14, 9,
        -10, 40, -33, 42, 51, -11, 28, -15,
        -12, 19, 33, -41, -38, 30, 1, -23,
        -58, -77, -82, -75, -24, -106, -38, -50
    }},

    {PieceType::Rook, {
        -31, -25, -19, 6, -3, -18, -31, -31,
        -52, -39, -31, -27, -30, -44, -44, -53,
        -43, -29, -41, -26, -24, -35, -27, -45,
        -27, -34, -16, -22, -13, -28, -47, -29,
        0, 4, 16, 14, 19, -5, -8, -7,
        20, 34, 29, 33, 46, 28, 24, 17,
        54, 28, 56, 69, 54, 61, 33, 61,
        35, 30, 34, 4, 38, 32, 55, 49
    }},

    {PieceType::Queen, {
        -40, -29, -31, -20, -20, -35, -35, -43,
        -35, -17, -1, -19, -14, -16, -22, -37,
        -30, -5, -14, -12, -17, -10, -15, -26,
        -15, -14, -2, -6, -2, -11, -21, -20,
        1, -15, 22, 16, 24, 19, -13, -7,
        -2, 44, 33, 61, 73, 63, 43, 3,
        15, 31, 62, -11, 19, 77, 57, 23,
        6, 2, -9, -103, 68, 25, 87, 28
    }},

    {PieceType::King, {
        16, 31, -2, -15, -15, -16, 40, 19,
        -3, 3, -14, -49, -56, -18, 12, 4,
        -47, -42, -42, -80, -65, -33, -28, -31,
        -55, -43, -53, -27, -52, -46, -8, -51,
        -56, 49, 10, -5, -20, 14, 1, -49,
        -61, 13, -58, 43, -68, 27, 37, -30,
        -32, 11, 54, 57, 55, 55, 10, 4,
        3, 53, 46, -100, -99, 60, 84, -63
    }}
};


int static_eval(ChessGame chess_game)
{
    int eval = 0;
    for (int i = 0; i < 64; i++)
    {
        Piece piece = chess_game.board[i];
        int square_score = 0;
        if (piece.color == Color::Nil) {continue;}
        else if (piece.color == Color::White)
        {
            square_score = piece_value.at(piece.piece_type) + square_value.at(piece.piece_type)[i];
        }
        else
        {
            square_score = -piece_value.at(piece.piece_type) - square_value.at(piece.piece_type)[63 - i];
        }
        eval += square_score;
    }
    return eval;
}


int minimax(ChessGame chess_game, int depth=3, int alpha=INT_MIN, int beta=INT_MAX)
{
    if (depth == 0)
    {
        return static_eval(chess_game);
    }
    std::vector<Move> moves = Chess::generateLegalMoves(chess_game);
    if (chess_game.player_turn == Color::White)
    {
        if (moves.empty()) {
            return Chess::isInCheck(chess_game) ? INT_MAX : 0;
        }
        int max_eval = INT_MIN;
        for (auto move: moves)
        {
            int eval = minimax(move.chess_game, depth - 1, alpha, beta);
            max_eval = std::max(max_eval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {break;}
        }
        return max_eval;
    }
    else
    {
        if (moves.empty()) {
            return Chess::isInCheck(chess_game) ? INT_MIN : 0;
        }
        int min_eval = INT_MAX;
        for (auto move: moves)
        {
            int eval = minimax(move.chess_game, depth - 1, alpha, beta);
            min_eval = std::min(min_eval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {break;}
        }
        return min_eval;
    }
}


Move Chess::botMove(ChessGame chess_game, std::vector<Move> moves)
{
    Move best_move = moves[0];
    int best_eval = minimax(moves[0].chess_game);
    for (int i = 1; i < moves.size(); i++)
    {
        int eval = minimax(moves[i].chess_game);
        if (chess_game.player_turn == Color::White && eval > best_eval)
        {
            best_eval = eval;
            best_move = moves[i];
        }
        else if (chess_game.player_turn == Color::Black && eval < best_eval)
        {
            best_eval = eval;
            best_move = moves[i];
        }
    }
    return best_move;
}