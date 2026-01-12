// screen_selection.hpp
#pragma once
#include <vector>
#include <string>
#include "hardware.hpp"
#include "chess.hpp"


enum class GameMode {
    Online,
    MultiplayerOffline,
    BotsOffline
};


enum class PlayerColor {
    White,
    Black
};


struct Settings
{
  GameMode game_mode; // Relevant for all gamemodes
  bool play_with_clock; // Relevant for local play
  int game_time_min;
  int extra_time_sec;
  int bot_strength;
  PlayerColor player_color; // Relevant for bots
};


Settings getGameSettings(void); // TO DO
PieceType screenSelectPromotion(const std::atomic<bool>&);
void displayBotPromotion(PieceType);
