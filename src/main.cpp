// main.cpp
#include <Arduino.h>
#include <thread>
#include <atomic>
#include "bluetooth.hpp"
#include "gamesettings.hpp"
#include "board.hpp"
#include "game.hpp"

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
  if (game_settings.game_mode != 'o' && game_settings.game_mode != 'm' && game_settings.game_mode != 'b')
  {
    return;
  }

  // ---Bluetooth Connect---
  if (game_settings.game_mode == 'o') // Online
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
  if (game_settings.game_mode == 'o')
  {
    game_settings = GameSettings::getWebAppSettings(); //REMOVE COMMENT WHEN IMPLEMENTED
  }


  // ---Start Game---
  Game::game(game_settings); //REMOVE COMMENT WHEN IMPLEMENTED
}

// Note sur comment print in std::string
//Serial.println(msg.c_str());