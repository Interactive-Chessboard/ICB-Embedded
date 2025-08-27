// structs.hpp
#pragma once
struct GameSettings
{
  int game_mode; // 1 = online, 2 = multiplayer offline, 3 = bots online
  bool player_color; //true = white, false = black
  int game_time_min; // Gmae time in minutes
  int extra_time_sec; // Extra time per move in seconds
  int bot_strenght; // Bots strenght in elo
};