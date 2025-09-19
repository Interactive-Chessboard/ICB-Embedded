// bluetooth.hpp
#pragma once
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

namespace Bluetooth 
{
std::string getBluetoothMessage(void);
void sendBluetoothMessage(std::string);
void init(void);
Winner getOnlineWinner();
void sendMove(Move);
void confirmConnection(void);
};