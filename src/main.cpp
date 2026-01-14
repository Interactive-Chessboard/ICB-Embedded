// main.cpp
#include <Arduino.h>
#include <chrono>
#include <thread>
#include <atomic>
#include "online_game/online_game.hpp"
#include "local_game/local_game.hpp"
#include "screen_selection/screen_selection.hpp"
#include "game_clock/game_clock.hpp"
#include "hardware.hpp"
#include "animations/animations.hpp"


void setup()
{
  Serial.begin(115200);
  Hardware::set(RealHardware::instance());
  Hardware::get().bluetooth_init();
  //delay(8000); //for debugging purposes
}

void loop()
{
  // ---Start up animation---
  playStartUpAnimation();

  // ---Game Settings---
  Settings game_settings = getGameSettings(); //REMOVE COMMENT WHEN IMPLEMENTED

  // ---Start Game Clock---
  std::thread clock_thread;
  ClockSetting clock_settings(game_settings.game_time_min, game_settings.extra_time_sec);
  std::atomic<bool> stop_clock_thread{false};
  clock_thread = std::thread(game_clock, std::ref(clock_settings), std::ref(stop_clock_thread));

  // ---Start game---
  if (game_settings.game_mode == GameMode::Online)
    onlineGame(clock_settings);
  else
    localGame(game_settings, clock_settings);

  // ---Stop Clock Thread;
  stop_clock_thread.store(true);
}

// Note sur comment print in std::string
//Serial.println(msg.c_str());

// Pour tester utilise
// MockHardware mock;
// Hardware::set(mock);


// TO DO
// Add unit tests

// TO DO
// Add api call to get version number

// TO DO
// Over the air update

// TO DO
// Chess bot