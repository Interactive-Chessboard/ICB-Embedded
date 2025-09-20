//game.cpp
#include "game.hpp"


ChessGame gameLoopOnline(Color player_color, ClockSetting &clock_settings)
{
    ChessGame chess_game;
    while (clock_settings.active)
    {
        Move move;
        std::vector<Move> legal_moves = LegalMoves::generateLegalMoves(chess_game);
        if (player_color == chess_game.player_turn)
        {
            move = Board::playerTurnDetectMoves(clock_settings, legal_moves); //REMOVE COMMENT WHEN IMPLEMENTED
            chess_game.winner = Bluetooth::sendMove(move);
        }
        else
        {
            std::pair<Move, Winner> result = Bluetooth::getOnlineMove(clock_settings, legal_moves);
            move = result.first;
            chess_game.winner = result.second;
            Board::playerMakeOpponentMove(clock_settings, move); //REMOVE COMMENT WHEN IMPLEMENTED
        }

        if (chess_game.winner != Winner::Nil)
        {
            clock_settings.active.store(false);
            break;
        }
        
        chess_game = move.chess_game;
    }
    return chess_game;
}


ChessGame gameLoopBotsOffline(Color player_color, ClockSetting &clock_settings)
{
    ChessGame chess_game;
    while (clock_settings.active)
    {
        // Generate a list of legal moves/positions and looks if the game has ended
        std::vector<Move> legal_moves = LegalMoves::generateLegalMoves(chess_game);
        chess_game.winner = GameEnd::calculateEndGame(chess_game, legal_moves);
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
            move = Bots::getBotMove(chess_game); //REMOVE COMMENT WHEN IMPLEMENTED
            Board::playerMakeOpponentMove(clock_settings, move); //REMOVE COMMENT WHEN IMPLEMENTED
        }

        chess_game = move.chess_game;  
    }
    return chess_game;
}



ChessGame gameLoopMultiplayerOffline(ClockSetting &clock_settings)
{
    ChessGame chess_game;
    while (clock_settings.active)
    {
        // Generate a list of legal moves/positions and looks if the game has ended
        std::vector<Move> legal_moves = LegalMoves::generateLegalMoves(chess_game);
        chess_game.winner = GameEnd::calculateEndGame(chess_game, legal_moves);
        if (chess_game.winner != Winner::Nil)
        {
            clock_settings.active.store(false);
            break;
        }

        // Make the move on the board
        Move move = Board::playerTurnDetectMoves(clock_settings, legal_moves); //REMOVE COMMENT WHEN IMPLEMENTED

        chess_game = move.chess_game;  
    }
    return chess_game;
}


void Game::game(Settings game_settings)
{
    // Set up the clock for the game
    int game_time_clock = game_settings.game_time_min * 6000;
    int game_time_extra_clock = game_settings.extra_time_sec * 100;
    ClockSetting clock_settings(game_time_clock, game_time_extra_clock);

    // ---Start Loop---
    ChessGame chess_game;
    std::thread clock_thread;
    switch (game_settings.game_mode)
    {
    case GameMode::Online:
    {
        clock_thread = std::thread(ChessClock::chess_clock_online, std::ref(clock_settings)); //REMOVE COMMENT WHEN IMPLEMENTED
        chess_game = gameLoopOnline(game_settings.player_color, std::ref(clock_settings));
        break;
    }
    case GameMode::BotsOffline:
    {
        clock_thread = std::thread(ChessClock::chess_clock_offline, std::ref(clock_settings)); //REMOVE COMMENT WHEN IMPLEMENTED
        chess_game = gameLoopBotsOffline(game_settings.player_color, std::ref(clock_settings));
        break;
    }
    case GameMode::MultiplayerOffline:
    {
        clock_thread = std::thread(ChessClock::chess_clock_offline, std::ref(clock_settings)); //REMOVE COMMENT WHEN IMPLEMENTED
        chess_game = gameLoopMultiplayerOffline(std::ref(clock_settings));
        break;
    }
    default:
        return;
    }

    // ---Game End---
    clock_thread.join();
    if (chess_game.winner == Winner::Nil)
    {
        chess_game.winner = GameEnd::getClockWinner(clock_settings, chess_game);
    }
    GameEnd::gameEndAnimation(chess_game.winner); //REMOVE COMMENT WHEN IMPLEMENTED
}
