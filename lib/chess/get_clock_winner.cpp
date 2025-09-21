// get_clock_winner.cpp
#include "chess.hpp"


Winner Chess::getClockWinner(const ClockSetting &clock_settings, const ChessGame &chess_game)
{
    if (clock_settings.time_white <= 0 && !insufficientMaterials(chess_game, Color::Black))
    {
        return Winner::Black;
    }
    if (clock_settings.time_black <= 0 && !insufficientMaterials(chess_game, Color::White))
    {
        return Winner::White;
    }
    return Winner::Draw;
}