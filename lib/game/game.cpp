//game.cpp
#include "game.hpp"

void game(Settings game_settings)
{
    std::atomic<bool> active(true);
    std::thread clock_thread(&chess_clock, game_settings, std::ref(active)); //REMOVE COMMENT WHEN IMPLEMENTED


    // ---Start Loop---
    char winner = gameLoop(game_settings, std::ref(active)); //REMOVE COMMENT WHEN IMPLEMENTED

    // ---Game End---
    clock_thread.join();
    gameEnd(winner); //REMOVE COMMENT WHEN IMPLEMENTED
}
