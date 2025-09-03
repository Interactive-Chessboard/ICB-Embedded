// get_web_app_settings.cpp
#include "game_settings.hpp"

Settings GameSettings::getWebAppSettings()
{
    Settings game_settings;
    game_settings.player_color = 'w';
    game_settings.game_time_min = 10;
    game_settings.extra_time_sec = 5;
    game_settings.bot_strenght = 500;
    return game_settings;
}