//game.cpp
#include "game.hpp"


Winner gameLoop(Settings game_settings, ClockSetting &clock_settings)
{
    ChessGame chess_game;
    Winner winner;
    while (clock_settings.active)
    {
        // Generate a list of legal moves/positions
        std::vector<Move> legal_moves = LegalMoves::generateLegalMoves(chess_game); //REMOVE COMMENT WHEN IMPLEMENTED
        
        // Calculates if the game is finished
        winner = GameEnd::calculateEndGame(chess_game, legal_moves); //REMOVE COMMENT WHEN IMPLEMENTED
        
        // Break if the game is finished
        if (winner != Winner::Nil)
        {
            clock_settings.active.store(false);
            break;
        }

        // Detect Move if it is the players turn
        if (game_settings.game_mode == GameMode::MultiplayerOffline || (game_settings.player_color == chess_game.player_turn))
        {
            Board::playerTurnDetectMoves(clock_settings, legal_moves); //REMOVE COMMENT WHEN IMPLEMENTED
        }

        // Receive the move and wait for the player to play it on the ICB if it is the opponent's turn
        else
        {
            Move move;
            if (game_settings.game_mode == GameMode::Online)
            {
                move = Opponent::getOnlineMove(clock_settings); //REMOVE COMMENT WHEN IMPLEMENTED
            }
            else if (game_settings.game_mode == GameMode::BotsOffline)
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
    // Set up the clock for the game
    int game_time_clock = game_settings.game_time_min * 6000;
    int game_time_extra_clock = game_settings.extra_time_sec * 100;
    ClockSetting clock_settings(game_time_clock, game_time_extra_clock);
    // Start the clock as a thread
    std::thread clock_thread(ChessClock::chess_clock, game_settings, std::ref(clock_settings)); //REMOVE COMMENT WHEN IMPLEMENTED


    // ---Start Loop---
    // Start the game loop
    Winner winner = gameLoop(game_settings, std::ref(clock_settings));

    // ---Game End---
    // Plays the end game animation
    clock_thread.join();
    GameEnd::gameEndAnimation(winner); //REMOVE COMMENT WHEN IMPLEMENTED
}
