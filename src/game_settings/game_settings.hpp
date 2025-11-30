// game_settings.hpp


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
  GameMode game_mode;
  PlayerColor player_color;
  int game_time_min;
  int extra_time_sec;
  int bot_strength;
};


Settings getGameSettings(void);