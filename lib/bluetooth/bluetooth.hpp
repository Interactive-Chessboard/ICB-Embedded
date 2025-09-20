// bluetooth.hpp
#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "structs.hpp"
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
Move getOnlineMove(ClockSetting&, Winner&, const std::vector<Move>&);
};