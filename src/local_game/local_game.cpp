// local_game.cpp
#include "local_game.hpp"


std::vector<Animation> createBasicAnimation(Winner color)
{
    return std::vector<Animation> {};
}

ChessGame gameLoopBotsOffline(PlayerColor gamemode_player_color, ClockSetting &clock_settings)
{
    Color player_color = (gamemode_player_color == PlayerColor::White ? Color::White : Color::Black);

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
            move = make_move.startOffline(std::ref(clock_settings));
        }
        else
        {
            move = Chess::botMove(chess_game, legal_moves);
            // TODO display promotion on screen

            uint64_t bitboard_move = Chess::getGameBitBoard(move.chess_game);
            SetBoard set_board(bitboard_move);
            set_board.startOffline(std::ref(clock_settings));
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
        Move move = make_move.startOffline(std::ref(clock_settings));

        chess_game = move.chess_game;
        clock_settings.player_turn.store(chess_game.player_turn);
        past_move = move;
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
        if (starting_position == Hardware::get().getBoardArr())
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
        chess_game.winner = Chess::getClockWinner(clock_settings.time_white.load(), clock_settings.time_black.load(), chess_game);
    }
    std::vector<Animation> basic_animations = createBasicAnimation(chess_game.winner);
    std::atomic<bool> stop{false};
    Hardware::get().playAnimations(stop, basic_animations);
}
