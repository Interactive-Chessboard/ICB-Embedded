// select.cpp
#include "select.hpp"

GameSettings Select::select()
{
    GameSettings game_settings;
    game_settings.game_mode = 2;
    game_settings.player_color = 1;
    game_settings.game_time_min = 10;
    game_settings.extra_time_sec = 5;
    game_settings.bot_strenght = 500;
    return game_settings;
}