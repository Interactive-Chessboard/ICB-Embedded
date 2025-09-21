// main.cpp
#include <Arduino.h>
#include "game.hpp"
#include "structs.hpp"

void setup() 
{
  // Set up a serial connection
  Serial.begin(115200);
  // Start the bluetooth server
  Bluetooth::init();
  //delay(8000); //for debugging purposes
}

void loop()
{
  // ---getGameSettings---
  // Get game settings through the screen and buttons
  Settings game_settings = Board::getGameSettings(); //REMOVE COMMENT WHEN IMPLEMENTED

  // ---Bluetooth Connect---
  // Confirm the bluetooth connection if playing online
  if (game_settings.game_mode == GameMode::Online && !Bluetooth::confirmConnection())
  {
    return;
  }

  // ---Init---
  // Waits until all pieces are placed in their starting position
  int timeout_reached = Board::waitForInitBoard(6000); //REMOVE COMMENT WHEN IMPLEMENTED
  if (timeout_reached)
  {
    return;
  }

  // ---Web App Settings---
  // Get game settings from the web app if playing online
  if (game_settings.game_mode == GameMode::Online)
  {
    game_settings = Bluetooth::getWebAppSettings(); //REMOVE COMMENT WHEN IMPLEMENTED
  }


  // ---Start Game---
  Game::game(game_settings);
}

// Note sur comment print in std::string
//Serial.println(msg.c_str());