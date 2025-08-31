//gameloop.cpp
#include "gameloop.hpp"

std::vector<Moves> generateLegalMoves(ChessGame chess_game) 
{
    std::vector<Moves> legal_moves;
    return legal_moves;
}
void playerTurnDetectMoves(ClockSetting &clock_settings, std::vector<Moves> legal_moves)
{
    
}

void OpponentTurnReceiveMoves(ClockSetting &clock_setting)
{
    
}

char calculateEndGame(ChessGame chess_game, std::vector<Moves> legal_moves)
{
    return ' ';
}

char gameLoop(Settings game_settings, ClockSetting &clock_settings)
{
    ChessGame chess_game;
    char winner;
    while (clock_settings.active)
    {
        std::vector<Moves> legal_moves = generateLegalMoves(chess_game);
        winner = calculateEndGame(chess_game, legal_moves);
        if (winner != ' ')
        {
            clock_settings.active.store(false);
            break;
        }
        if (game_settings.game_mode == 2 || (game_settings.player_color == chess_game.player_turn))
        {
            playerTurnDetectMoves(clock_settings, legal_moves);
        }
        else
        {
            OpponentTurnReceiveMoves(clock_settings);
        }
        
    }
    return winner;
}

