// settings.hpp
#pragma once
struct Settings
{
  char game_mode; // 'o' = online, 'm' = multiplayer offline, 'b' = bots onffline
  char player_color; //'w' = white, 'b' = black
  int game_time_min; // Game time in minutes
  int extra_time_sec; // Extra time per move in seconds
  int bot_strenght; // Bots strenght in elo
};