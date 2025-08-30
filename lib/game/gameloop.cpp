//gameloop.cpp
#include "gameloop.hpp"

struct ChessGame
{
    char player_turn;
    char board[64];
    char castle[4];
    int enPassant;
};

void playerTurn()
{
    char temp = ' ';
}

void OpponentTurn()
{
    char temp = ' ';
}

bool calculateEndGame(ChessGame chess_game)
{
    char temp = ' ';
    return true;
}

char gameLoop(Settings game_settings, std::atomic<bool>& active)
{
    ChessGame chess_game;
    while (active)
    {
        if (game_settings.game_mode == 2 || (game_settings.player_color == chess_game.player_turn))
        {
            playerTurn();
        }
        else
        {
            OpponentTurn();
        }
        if (calculateEndGame(chess_game))
        {
            active.store(false);
        }
    }
    return 'w';
}

