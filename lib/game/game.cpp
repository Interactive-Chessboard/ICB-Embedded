//game.cpp
#include "game.hpp"

void game(Settings game_settings)
{
    int game_time_clock = game_settings.game_time_min * 6000;
    int game_time_extra_clock = game_settings.extra_time_sec * 100;
    ClockSetting clock_settings(game_time_clock, game_time_extra_clock);
    std::thread clock_thread(&chess_clock, game_settings, std::ref(clock_settings)); //REMOVE COMMENT WHEN IMPLEMENTED


    // ---Start Loop---
    char winner = gameLoop(game_settings, std::ref(clock_settings)); //REMOVE COMMENT WHEN IMPLEMENTED

    // ---Game End---
    clock_thread.join();
    gameEnd(winner); //REMOVE COMMENT WHEN IMPLEMENTED
}
