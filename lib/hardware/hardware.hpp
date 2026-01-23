// hardware.hpp
#pragma once
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
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


struct LedColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    LedColor(int red = 0, int green = 0, int blue = 0) :
             red(red), green(green), blue(blue) {}

    bool operator==(const LedColor& other) const {
        return red == other.red &&
               green == other.green &&
               blue == other.blue;
    }
};


// Interface class
class IHardware
{
public:
    virtual ~IHardware() = default;

    // bluetooth
    virtual void bluetooth_init() = 0;
    virtual std::string getBluetoothMessage() = 0;
    virtual void sendBluetoothMessage(std::string) = 0;

    // board
    virtual uint64_t getBoardArr() = 0;

    // lights
    virtual void setLed(std::array<LedColor, 64>) = 0;
    virtual void clearLed() = 0;

    // screen
    virtual void setTime(int, int) = 0;
    virtual bool detectSelectClick() = 0;
    virtual bool detectStartClick() = 0;
    virtual void setScreen(std::vector<std::string>, int) = 0;
    virtual void clearScreen() = 0;
};


// Real Hardware
class RealHardware : public IHardware
{
private:
    RealHardware() = default;
public:
    static RealHardware& instance()
    {
        static RealHardware instance;
        return instance;
    }

    RealHardware(const RealHardware&) = delete;
    RealHardware& operator=(const RealHardware&) = delete;

    void bluetooth_init() override;
    std::string getBluetoothMessage() override;
    void sendBluetoothMessage(std::string) override;

    uint64_t getBoardArr() override;

    void setLed(std::array<LedColor, 64>) override;
    void clearLed() override;

    void setTime(int, int) override;
    bool detectSelectClick() override;
    bool detectStartClick() override;
    void setScreen(std::vector<std::string>, int) override;
    void clearScreen() override;
};


class Hardware
{
public:
    static IHardware& get()
    {
        return *instance_;
    }

    static void set(IHardware& hw)
    {
        instance_ = &hw;
    }

private:
    static IHardware* instance_;
};
