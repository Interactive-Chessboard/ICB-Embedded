// get_clock_winner.cpp
#include "chess.hpp"


Winner Chess::getClockWinner(int time_white, int time_black, const ChessGame &chess_game)
{
    if (time_white <= 0 && !insufficientMaterials(chess_game, Color::Black))
    {
        return Winner::Black;
    }
    if (time_black <= 0 && !insufficientMaterials(chess_game, Color::White))
    {
        return Winner::White;
    }
    return Winner::Draw;
}
