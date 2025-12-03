//game.cpp
#include "local_game.hpp"
/*

ChessGame gameLoopBotsOffline(Color player_color, ClockSetting &clock_settings)
{
    ChessGame chess_game;
    while (clock_settings.active.load())
    {
        // Generate a list of legal moves/positions and looks if the game has ended
        std::vector<Move> legal_moves = Chess::generateLegalMoves(chess_game);
        chess_game.winner = Chess::calculateEndGame(chess_game, legal_moves);
        if (chess_game.winner != Winner::Nil)
        {
            clock_settings.active.store(false);
            break;
        }

        Move move;
        if (player_color == chess_game.player_turn)
        {
            move = Board::playerTurnDetectMoves(clock_settings, legal_moves); //REMOVE COMMENT WHEN IMPLEMENTED
        }
        else
        {
            move = Chess::botMove(chess_game, legal_moves);
            Board::playerMakeOpponentMove(clock_settings, move); //REMOVE COMMENT WHEN IMPLEMENTED
        }

        chess_game = move.chess_game;
        clock_settings.player_turn.store(chess_game.player_turn); 
    }
    return chess_game;
}



ChessGame gameLoopMultiplayerOffline(ClockSetting &clock_settings)
{
    ChessGame chess_game;
    while (clock_settings.active.load())
    {
        // Generate a list of legal moves/positions and looks if the game has ended
        std::vector<Move> legal_moves = Chess::generateLegalMoves(chess_game);
        chess_game.winner = Chess::calculateEndGame(chess_game, legal_moves);
        if (chess_game.winner != Winner::Nil)
        {
            clock_settings.active.store(false);
            break;
        }

        // Make the move on the board
        Move move = Board::playerTurnDetectMoves(clock_settings, legal_moves); //REMOVE COMMENT WHEN IMPLEMENTED

        chess_game = move.chess_game;
        clock_settings.player_turn.store(chess_game.player_turn); 
    }
    return chess_game;
}


void localGame(Settings game_settings)
{
    // Set up the clock for the game
    int game_time_clock = game_settings.game_time_min * 6000;
    int game_time_extra_clock = game_settings.extra_time_sec * 100;
    ClockSetting clock_settings(game_time_clock, game_time_extra_clock);

    // ---Start Loop---
    ChessGame chess_game;
    std::thread clock_thread;
    clock_thread = std::thread(Chess::chess_clock, std::ref(clock_settings));
    switch (game_settings.game_mode)
    {
    case GameMode::BotsOffline:
        chess_game = gameLoopBotsOffline(game_settings.player_color, std::ref(clock_settings));
        break;
    case GameMode::MultiplayerOffline:
        chess_game = gameLoopMultiplayerOffline(std::ref(clock_settings));
        break;
    default:
        return;
    }

    // ---Game End---
    clock_thread.join();
    if (chess_game.winner == Winner::Nil)
    {
        chess_game.winner = Chess::getClockWinner(clock_settings, chess_game);
    }
    Board::gameEndAnimation(chess_game.winner); //REMOVE COMMENT WHEN IMPLEMENTED
}
    */
