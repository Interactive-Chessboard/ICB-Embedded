// local_game.cpp
#include "local_game.hpp"


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


void localGame(Settings game_settings, ClockSetting &clock_settings)
{
    // ---Init---
    u_int64_t starting_position = 0xffff00000000ffff;
    int timeout_ms = 600000;
    bool timeout_reached = true;
    for (int i = 0; i < timeout_ms; i++)
    {
        if (starting_position == Board::getBoardArr())
        {
        timeout_reached = false;
        break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    if (timeout_reached)
        return;

    // ---Start Loop---
    ChessGame chess_game;
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
    if (chess_game.winner == Winner::Nil)
    {
        //chess_game.winner = Chess::getClockWinner(clock_settings, chess_game);
    }
    //Board::gameEndAnimation(chess_game.winner); //REMOVE COMMENT WHEN IMPLEMENTED
}

