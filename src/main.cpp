// main.cpp
#include <Arduino.h>
#include "game.hpp"
#include "structs.hpp"

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

  // ---Bluetooth Connect---
  if (game_settings.game_mode == GameMode::Online && !Bluetooth::confirmConnection())
  {
    return;
  }

  // ---Init---
  int timeout_reached = Board::waitForInitBoard(6000); //REMOVE COMMENT WHEN IMPLEMENTED
  if (timeout_reached)
  {
    return;
  }

  // ---Web App Settings---
  if (game_settings.game_mode == GameMode::Online)
  {
    game_settings = Bluetooth::getWebAppSettings();
  }

  // ---Start Game---
  Game::game(game_settings);
}

// Note sur comment print in std::string
//Serial.println(msg.c_str());