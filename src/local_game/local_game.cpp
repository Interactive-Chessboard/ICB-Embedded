// local_game.cpp
#include "local_game.hpp"


ChessGame gameLoopBotsOffline(Color player_color, ClockSetting &clock_settings, BotStrength bot_strength)
{
    ChessGame chess_game;
    Move past_move;
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
            MakeMove make_move(chess_game, legal_moves, past_move.from_square, past_move.to_square);
            move = make_move.startOffline(clock_settings.active);
        }
        else
        {
            move = Chess::botMove(chess_game, legal_moves, bot_strength);

            if (move.promotion != Piece())
                displayBotPromotion(move.promotion.piece_type);

            uint64_t bitboard_move = Chess::getGameBitBoard(move.chess_game);
            SetBoard set_board(bitboard_move);
            set_board.startOffline(clock_settings.active);
            if (move.promotion != Piece())
                Hardware::get().reserveScreen(false);
        }

        chess_game = move.chess_game;
        clock_settings.player_turn.store(chess_game.player_turn);
        past_move = move;
    }
    return chess_game;
}



ChessGame gameLoopMultiplayerOffline(ClockSetting &clock_settings)
{
    ChessGame chess_game;
    Move past_move;
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
        MakeMove make_move(chess_game, legal_moves, past_move.from_square, past_move.to_square);
        Move move = make_move.startOffline(clock_settings.active);

        chess_game = move.chess_game;
        clock_settings.player_turn.store(chess_game.player_turn);
        past_move = move;
    }
    return chess_game;
}


void localGame(Settings game_settings, ClockSetting &clock_settings)
{
    // ---Init---
    uint64_t starting_position = 0xffff00000000ffffULL;
    int timeout_ms = 600000;
    bool timeout_reached = true;
    for (int i = 0; i < timeout_ms; i++)
    {
        if (starting_position == Hardware::get().getBoardArr())
        {
        timeout_reached = false;
        break;
        }
        // TODO set lights for pieces in the wrong place
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    if (timeout_reached)
        return;

    // ---Start Loop---
    ChessGame chess_game;
    switch (game_settings.game_mode)
    {
    case GameMode::BotsOffline:
        chess_game = gameLoopBotsOffline(game_settings.player_color, clock_settings, game_settings.bot_strength);
        break;
    case GameMode::MultiplayerOffline:
        chess_game = gameLoopMultiplayerOffline(clock_settings);
        break;
    default:
        return;
    }

    // ---Game End---
    if (chess_game.winner == Winner::Nil)
    {
        chess_game.winner = Chess::getClockWinner(clock_settings.time_white.load(), clock_settings.time_black.load(), chess_game);
    }
    playWinnerAnimation(chess_game, 5, 1000);
}
