// bluetooth.cpp
#include "hardware.hpp"

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
std::queue<std::string> receive_queue;
std::queue<std::string> send_queue;


/**
 * @brief Blocks until a message is available, then returns and removes it.
 *
 * Used by both the send and receive queues. Sleeps briefly while waiting.
 *
 * @param queue Reference to the queue to read from.
 * @return The next string from the queue.
 */
std::string getFromQueue(std::queue<std::string> &queue)
{
    std::string msg;
    while(true)
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


/**
 * @brief BLE callback handler invoked when a client writes to the characteristic.
 *
 * When data is written by the client:
 *  - it is queued internally,
 *  - the next message in the send queue is retrieved,
 *  - the response is sent back via BLE notify().
 */
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


/**
 * @brief Initialize the Bluetooth device, BLE server, service and characteristic.
 *
 * Creates the BLE server, registers the characteristic, sets callbacks,
 * and starts advertising to make the device discoverable.
 */
void RealHardware::bluetooth_init()
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


/**
 * @brief Blocking read from the received BLE message queue.
 *
 * This function blocks until a message has arrived from the BLE client.
 *
 * @return The received message as a std::string.
 */
std::string RealHardware::getBluetoothMessage()
{
    return getFromQueue(receive_queue);
}


/**
 * @brief Queue a message to be sent back to the BLE client.
 *
 * The next time a client writes data, this queued message will be sent back
 * as a notification to the client.
 *
 * @param msg Message to enqueue for transmission.
 */
void RealHardware::sendBluetoothMessage(std::string msg)
{
    send_queue.push(msg);
}
