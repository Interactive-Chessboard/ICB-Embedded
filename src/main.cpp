// main.cpp
#include <Arduino.h>
#include "online_game/online_game.hpp"
#include "local_game/local_game.hpp"
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
  Settings game_settings = Board::getGameSettings(); //REMOVE COMMENT WHEN IMPLEMENTED

  // ---Start online game---
  if (game_settings.game_mode == GameMode::Online)
  {
    onlineGame();
    return;
  }

  // ---Init---
  int timeout_reached = Board::waitForInitBoard(6000); //REMOVE COMMENT WHEN IMPLEMENTED
  if (timeout_reached)
    return;

  // ---Local Game---
  localGame(game_settings);
}

// Note sur comment print in std::string
//Serial.println(msg.c_str());