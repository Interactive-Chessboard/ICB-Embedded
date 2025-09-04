// main.cpp
#include <Arduino.h>
#include "bluetooth.hpp"
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
  Settings game_settings = GameSettings::getGameSettings(); //REMOVE COMMENT WHEN IMPLEMENTED

  // ---Bluetooth Connect---
  if (game_settings.game_mode == GameMode::Online)
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
  if (game_settings.game_mode == GameMode::Online)
  {
    game_settings = GameSettings::getWebAppSettings(); //REMOVE COMMENT WHEN IMPLEMENTED
  }


  // ---Start Game---
  Game::game(game_settings); //REMOVE COMMENT WHEN IMPLEMENTED
}

// Note sur comment print in std::string
//Serial.println(msg.c_str());