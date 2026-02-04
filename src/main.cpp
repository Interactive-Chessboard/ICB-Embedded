// main.cpp
#ifdef ARDUINO
#include <Arduino.h>
#include <chrono>
#include <thread>
#include <atomic>
#include "online_game/online_game.hpp"
#include "local_game/local_game.hpp"
#include "screen_selection/screen_selection.hpp"
#include "game_clock/game_clock.hpp"
#include "hardware.hpp"
#include "real_hardware.hpp"
#include "animations/animations.hpp"


#ifndef UNIT_TEST
void setup()
{
  Serial.begin(115200);

  // ---Start up animation---
  playStartUpAnimation(1000);
}

void loop()
{
  // ---Game Settings---
  Settings game_settings = getGameSettings();

  // ---Start Game Clock---
  ClockSetting clock_settings(game_settings.game_time_min, game_settings.extra_time_sec);
  std::atomic<bool> stop_clock_thread{false};
  std::thread clock_thread = std::thread(startGameClock, std::ref(clock_settings), std::ref(stop_clock_thread));

  // ---Start game---
  if (game_settings.game_mode == GameMode::Online)
    onlineGame(clock_settings);
  else
    localGame(game_settings, clock_settings);

  // ---Stop Clock Thread;
  stop_clock_thread.store(true);
  clock_thread.join();
}
#else
int main() {return 0;}
#endif
#endif
