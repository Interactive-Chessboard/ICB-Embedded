//game.cpp
#include "game.hpp"


Winner gameLoopMultiplayerOffline(Settings game_settings, ClockSetting &clock_settings)
{
    ChessGame chess_game;
    Winner winner = Winner::Nil;
    while (clock_settings.active)
    {
        // Generate a list of legal moves/positions
        std::vector<Move> legal_moves = LegalMoves::generateLegalMoves(chess_game);
        
        // Calculates if the game is finished
        if (game_settings.game_mode != GameMode::Online)
        {
            winner = GameEnd::calculateEndGame(chess_game, legal_moves); //REMOVE COMMENT WHEN IMPLEMENTED
        } 
        
        // Break if the game is finished
        if (winner != Winner::Nil)
        {
            clock_settings.active.store(false);
            break;
        }

        Move move = Board::playerTurnDetectMoves(clock_settings, legal_moves); //REMOVE COMMENT WHEN IMPLEMENTED
        
        // Update the settings of the game to prepare for next move
        chess_game = move.chess_game;
        
    }
    return winner;
}


Winner gameLoopBotsOffline(Settings game_settings, ClockSetting &clock_settings)
{
    ChessGame chess_game;
    Winner winner = Winner::Nil;
    while (clock_settings.active)
    {
        // Generate a list of legal moves/positions
        std::vector<Move> legal_moves = LegalMoves::generateLegalMoves(chess_game);

        Move move;
        // Detect Move if it is the players turn
        if (game_settings.player_color == chess_game.player_turn)
        {
            move = Board::playerTurnDetectMoves(clock_settings, legal_moves); //REMOVE COMMENT WHEN IMPLEMENTED
        }

        // Break if the game is finished
        winner = GameEnd::calculateEndGame(chess_game, legal_moves); //REMOVE COMMENT WHEN IMPLEMENTED
        if (winner != Winner::Nil)
        {
            clock_settings.active.store(false);
            break;
        }

        // Receive the move and wait for the player to play it on the ICB if it is the opponent's turn
        else
        {
            move = Opponent::getBotMove(clock_settings); //REMOVE COMMENT WHEN IMPLEMENTED
            Board::playerMakeOpponentMove(clock_settings, move); //REMOVE COMMENT WHEN IMPLEMENTED
        }
        
        // Update the settings of the game to prepare for next move
        chess_game = move.chess_game;
        
    }
    return winner;
}


Winner gameLoopOnline(Settings game_settings, ClockSetting &clock_settings)
{
    ChessGame chess_game;
    Winner winner = Winner::Nil;
    while (clock_settings.active)
    {
        // Generate a list of legal moves/positions
        std::vector<Move> legal_moves = LegalMoves::generateLegalMoves(chess_game);

        Move move;
        // Detect Move if it is the players turn
        if (game_settings.player_color == chess_game.player_turn)
        {
            move = Board::playerTurnDetectMoves(clock_settings, legal_moves); //REMOVE COMMENT WHEN IMPLEMENTED
            Bluetooth::sendMove(move); //REMOVE COMMENT WHEN IMPLEMENTED
        }

        // Receive the move and wait for the player to play it on the ICB if it is the opponent's turn
        else
        {
            move = Opponent::getOnlineMove(clock_settings); //REMOVE COMMENT WHEN IMPLEMENTED
            Board::playerMakeOpponentMove(clock_settings, move); //REMOVE COMMENT WHEN IMPLEMENTED
        }

        winner = Bluetooth::getOnlineWinner(); //REMOVE COMMENT WHEN IMPLEMENTED
        if (winner != Winner::Nil)
        {
            clock_settings.active.store(false);
            break;
        }
        
        // Update the settings of the game to prepare for next move
        chess_game = move.chess_game;
        
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
    Winner winner;
    switch (game_settings.game_mode)
    {
    case GameMode::Online:
        winner = gameLoopOnline(game_settings, std::ref(clock_settings));
        break;
    
    case GameMode::BotsOffline:
        winner = gameLoopBotsOffline(game_settings, std::ref(clock_settings));
        break;

    case GameMode::MultiplayerOffline:
        winner = gameLoopMultiplayerOffline(game_settings, std::ref(clock_settings));
        break;
    default:
        return;
    }

    // ---Game End---
    // Plays the end game animation
    clock_thread.join();
    GameEnd::gameEndAnimation(winner); //REMOVE COMMENT WHEN IMPLEMENTED
}
