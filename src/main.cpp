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

#ifndef FW_VERSION
#define FW_VERSION "0.0.0"
#endif

#ifndef UNIT_TEST
void setup()
{
  Serial.begin(115200);

  // temporary
  delay(8000); // allow serial to come up

  Serial.println();
  Serial.println("================================");
  Serial.print("Firmware version: ");
  Serial.println(FW_VERSION);
  Serial.println("================================");

  Hardware::set(RealHardware::instance());
  Hardware::get().bluetoothInit();
  //delay(8000); //for debugging purposes
}

void loop()
{
  // ---Start up animation---
  playStartUpAnimation();

  // ---Game Settings---
  Settings game_settings = getGameSettings(); //REMOVE COMMENT WHEN IMPLEMENTED

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