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

/**
 * @brief Namespace encapsulating Bluetooth functionality for BLE messaging.
 */
namespace Bluetooth 
{
/**
 * @brief Initialize the Bluetooth device, BLE server, service and characteristic.
 *
 * Creates the BLE server, registers the characteristic, sets callbacks,
 * and starts advertising to make the device discoverable.
 */
void init(void);

/**
 * @brief Blocking read from the received BLE message queue.
 *
 * This function blocks until a message has arrived from the BLE client.
 *
 * @return The received message as a std::string.
 */
std::string getBluetoothMessage(void);

/**
 * @brief Queue a message to be sent back to the BLE client.
 *
 * The next time a client writes data, this queued message will be sent back
 * as a notification to the client.
 *
 * @param msg Message to enqueue for transmission.
 */
void sendBluetoothMessage(std::string);
};