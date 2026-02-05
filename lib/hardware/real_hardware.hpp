// real_hardware.hpp
#pragma once
#ifdef ARDUINO
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <queue>
#include <thread>
#include <vector>
#include <chrono>
#include <string>
#include <atomic>
#include <array>
#include <mutex>
#include <cstdio>
#include "esp_ota_ops.h"
#include "hardware_interface.hpp"
#include "esp_system.h"

#ifndef FW_VERSION
#define FW_VERSION "0.0.0"
#endif

#define MSG_SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define MSG_CHAR_UUID           "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define SERVICE_UUID_OTA        "c8659210-af91-4ad3-a995-a58d6fd26145"
#define CHAR_UUID_OTA_FW        "c8659211-af91-4ad3-a995-a58d6fd26145"
#define CHAR_UUID_HW_VERSION    "c8659212-af91-4ad3-a995-a58d6fd26145"

#define FULL_PACKET             512


// Real Hardware
class RealHardware : public IHardware
{
private:
    RealHardware() = default;

    BLEServer* pServer = nullptr;
    BLECharacteristic* msgChar = nullptr;
    BLECharacteristic* otaChar = nullptr;
    BLECharacteristic* versionChar = nullptr;
    std::queue<std::string> rxQueue;
    esp_ota_handle_t otaHandle = 0;
    volatile bool otaInProgress = false;
    void handleOtaWrite(const std::string&) override;

    std::mutex screen_mutex;
    bool screen_reserved = false;
    std::array<LedColor, 64> current_global_light;
public:
    static RealHardware& instance()
    {
        static RealHardware instance;
        return instance;
    }

    RealHardware(const RealHardware&) = delete;
    RealHardware& operator=(const RealHardware&) = delete;

    std::string getFirmwareVersion() override;
    std::string getMacAddress() override;

    void bluetoothInit() override;
    std::string getBluetoothMessage() override;
    void sendBluetoothMessage(const std::string&) override;
    friend class MessageCallbacks;
    friend class OtaCallbacks;

    uint64_t getBoardArr() override;

    void setLed(const std::array<LedColor, 64>&) override;
    void clearLed() override;

    void reserveScreen(bool r) override;
    void setTimeScreen(const std::vector<std::string>&) override;
    void setScreen(const std::vector<std::string>&, int) override;
    void clearScreen() override;

    bool detectSelectClick() override;
    bool detectStartClick() override;
};

#endif
