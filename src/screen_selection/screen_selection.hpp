// screen_selection.hpp
#pragma once
#include <vector>
#include <string>
#include <cstdlib>
#include "hardware.hpp"
#include "chess.hpp"


enum class GameMode {
    Online,
    MultiplayerOffline,
    BotsOffline
};


struct Settings
{
  GameMode game_mode; // Relevant for all gamemodes
  Color player_color;
  bool play_with_clock; // Relevant for local play
  int game_time_min;
  int extra_time_sec;
  BotStrength bot_strength; // Relevant for bots
};


Settings getGameSettings(void);
PieceType screenSelectPromotion(const std::atomic<bool>&);
void displayBotPromotion(PieceType);
