// main.cpp
#include <Arduino.h>
#include <thread>
#include <atomic>
#include "structs.hpp"
#include "bluetooth.hpp"
#include "select.hpp"
#include "board.hpp"
#include "webbappsetting.hpp"
#include "clock.hpp"
#include "gameloop.hpp"
#include "gameend.hpp"

void setup() 
{
  Serial.begin(115200);
  Bluetooth::init();
  //delay(8000); //for debugging purposes
}

void loop() 
{
  // ---Selector---
  GameSettings game_settings;
  game_settings = Select::select(); //REMOVE COMMENT WHEN IMPLEMENTED
  if (game_settings.game_mode != 1 && game_settings.game_mode != 2 && game_settings.game_mode != 3)
  {
    return;
  }
  if (game_settings.game_mode == 1) // Online
  {
    std::string msg = Bluetooth::getBluetoothMessage();
    if (msg != "Connection Confirmed")
    {
      Bluetooth::sendBluetoothMessage("ERROR");
      return;
    }
    Bluetooth::sendBluetoothMessage("ACK");   
  }

  // ---Init---
  int timeout_reached = Board::waitForInitBoard(6000); //REMOVE COMMENT WHEN IMPLEMENTED
  if (timeout_reached)
  {
    return;
  }

  // ---Web App Settings---
  if (game_settings.game_mode == 1)
  {
    game_settings = WebAppSettings::getWebAppSettings(); //REMOVE COMMENT WHEN IMPLEMENTED
  }


  // ---Start Game---
  std::atomic<bool> active(true);
  std::thread clock_thread(Clock::clock, game_settings, std::ref(active)); //REMOVE COMMENT WHEN IMPLEMENTED


  // ---Start Loop---
  char winner = GameLoop::gameLoop(game_settings, std::ref(active)); //REMOVE COMMENT WHEN IMPLEMENTED

  // ---Game End---
  clock_thread.join();
  GameEnd::gameEnd(winner); //REMOVE COMMENT WHEN IMPLEMENTED


}

// Note sur comment print in std::string
//Serial.println(msg.c_str());