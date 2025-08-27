// bluetooth.cpp
#include "bluetooth.hpp"
#include <queue>
#include <thread>

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
std::queue<std::string> receive_queue;
std::queue<std::string> send_queue;


std::string getFromQueue(std::queue<std::string> &queue)
{
    std::string msg = "Error";
    int time_out_iterations = 12000; // number of 10 miliseconds in 2 min
    for(int i = 0; i < time_out_iterations; i++) 
    {
        if (!queue.empty()) 
        {
            msg = queue.front();
            queue.pop();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return msg;
}



// Callback to handle writes from the BLE client
class MyCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pChar) 
    {
        std::string value = pChar->getValue();
        if (value.length() > 0) 
        {
            receive_queue.push(value);
            std::string msg = getFromQueue(send_queue);
            pChar->setValue(msg);
            pChar->notify();  // send notification to client
        }
    }
};



void Bluetooth::sendBluetoothMessage(std::string msg)
{
    send_queue.push(msg);
}


std::string Bluetooth::getBluetoothMessage()
{
    return getFromQueue(receive_queue);
}


void Bluetooth::init()
{
    BLEDevice::init("ICB_test_board");
    pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);

    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_WRITE |
                        BLECharacteristic::PROPERTY_NOTIFY
                        );

    pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->setValue("Hello World!");
    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    BLEDevice::startAdvertising();
};