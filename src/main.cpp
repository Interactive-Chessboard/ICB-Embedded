// main.cpp
#include <Arduino.h>
#include <chrono>
#include <thread>
#include "online_game/online_game.hpp"
#include "local_game/local_game.hpp"
#include "game_settings/game_settings.hpp"
#include "structs.hpp"
#include "bluetooth.hpp"
#include "board.hpp"

void setup() 
{
  Serial.begin(115200);
  Bluetooth::init();
  //delay(8000); //for debugging purposes
}

void loop()
{
  // ---getGameSettings---
  Settings game_settings = getGameSettings(); //REMOVE COMMENT WHEN IMPLEMENTED

  // ---Start online game---
  if (game_settings.game_mode == GameMode::Online)
  {
    onlineGame(); //REMOVE COMMENT WHEN IMPLEMENTED
    return;
  }

  // ---Init---
  u_int64_t starting_position = 0xffff00000000ffff;
  int timeout_ms = 600000;
  bool timeout_reached = true;
  for (int i = 0; i < timeout_ms; i++)
  {
    if (starting_position == Board::getBoardArr())
    {
      timeout_reached = false;
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  if (timeout_reached)
    return;

  // ---Local Game---
  localGame(game_settings); //REMOVE COMMENT WHEN IMPLEMENTED
}

// Note sur comment print in std::string
//Serial.println(msg.c_str());