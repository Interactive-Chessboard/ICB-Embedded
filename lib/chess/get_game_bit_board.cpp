// get_game_bit_board.cpp
#include "chess.hpp"


uint64_t Chess::getGameBitBoard(ChessGame game)
{
    uint64_t bit_board = 0;
    for (int i = 0; i < 64; i++)
    {
        if (game.board.at(i) != Piece())
            bit_board |= (1ULL << i);
    }
    return bit_board;
}
