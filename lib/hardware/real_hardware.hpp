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
#include "hardware_interface.hpp"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


// Real Hardware
class RealHardware : public IHardware
{
private:
    RealHardware() = default;
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

    void bluetoothInit() override;
    std::string getBluetoothMessage() override;
    void sendBluetoothMessage(std::string) override;

    uint64_t getBoardArr() override;

    void setLed(std::array<LedColor, 64>) override;
    void clearLed() override;

    void reserveScreen(bool r) override;
    void setTimeScreen(std::vector<std::string>) override;
    void setScreen(std::vector<std::string>, int) override;
    bool detectSelectClick() override;
    bool detectStartClick() override;
};

#endif
