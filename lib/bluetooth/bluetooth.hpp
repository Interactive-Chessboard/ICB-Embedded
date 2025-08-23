// bluetooth.hpp
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

extern BLEServer *pServer;
extern BLECharacteristic *pCharacteristic;


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

namespace Bluetooth 
{


// Callback to handle writes from the BLE client
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pChar) {
    std::string value = pChar->getValue();

    if (value.length() > 0) {
      Serial.print("Received: ");
      Serial.println(value.c_str());

      // Echo the value back to the client
      pChar->setValue("Messaged received on esp32");
      pChar->notify();  // send notification to client
    }
  }
};

void init(void);

};