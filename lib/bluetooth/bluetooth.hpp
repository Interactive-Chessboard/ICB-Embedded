// bluetooth.hpp
#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "structs.hpp"
#include <queue>
#include <thread>
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

namespace Bluetooth 
{
void init(void);
std::string getBluetoothMessage(void);
void sendBluetoothMessage(std::string);
std::string extractValue(const std::string&, const std::string&);
bool confirmConnection(void);
Winner sendMove(const Move&);
std::pair<Move, Winner> getOnlineMove(ClockSetting&, const std::vector<Move>&);
};