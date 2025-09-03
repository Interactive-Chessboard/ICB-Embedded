//game.cpp
#include "game.hpp"


char gameLoop(Settings game_settings, ClockSetting &clock_settings)
{
    ChessGame chess_game;
    char winner;
    while (clock_settings.active)
    {
        std::vector<Move> legal_moves = LegalMoves::generateLegalMoves(chess_game); //REMOVE COMMENT WHEN IMPLEMENTED
        winner = GameEnd::calculateEndGame(chess_game, legal_moves); //REMOVE COMMENT WHEN IMPLEMENTED
        if (winner != ' ')
        {
            clock_settings.active.store(false);
            break;
        }
        if (game_settings.game_mode == 'm' || (game_settings.player_color == chess_game.player_turn))
        {
            Board::playerTurnDetectMoves(clock_settings, legal_moves); //REMOVE COMMENT WHEN IMPLEMENTED
        }
        else
        {
            Move move;
            if (game_settings.game_mode == 'o')
            {
                move = Opponent::getOnlineMove(clock_settings); //REMOVE COMMENT WHEN IMPLEMENTED
            }
            else
            {
                move = Opponent::getBotMove(clock_settings); //REMOVE COMMENT WHEN IMPLEMENTED
            }
            Board::playerMakeOpponentMove(clock_settings, move); //REMOVE COMMENT WHEN IMPLEMENTED
        }
        
    }
    return winner;
}


void Game::game(Settings game_settings)
{
    int game_time_clock = game_settings.game_time_min * 6000;
    int game_time_extra_clock = game_settings.extra_time_sec * 100;
    ClockSetting clock_settings(game_time_clock, game_time_extra_clock);
    std::thread clock_thread(ChessClock::chess_clock, game_settings, std::ref(clock_settings)); //REMOVE COMMENT WHEN IMPLEMENTED


    // ---Start Loop---
    char winner = gameLoop(game_settings, std::ref(clock_settings));

    // ---Game End---
    clock_thread.join();
    GameEnd::gameEndAnimation(winner); //REMOVE COMMENT WHEN IMPLEMENTED
}
