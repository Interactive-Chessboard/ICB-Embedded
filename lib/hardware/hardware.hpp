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
    u_int8_t red;
    u_int8_t green;
    u_int8_t blue;
    LedColor(int red = 0, int green = 0, int blue = 0) :
             red(red), green(green), blue(blue) {}

    bool operator==(const LedColor& other) const {
        return red == other.red &&
               green == other.green &&
               blue == other.blue;
    }
};


struct Animation {
    std::array<LedColor, 64> leds;
    int display_time_ms;
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
    virtual std::string playAnimations(std::atomic<bool>&, const std::vector<Animation>&) = 0;

    // screen
    virtual void setTime(int, int) = 0;
    virtual void setSentence(std::string) = 0;
    virtual void clearBoard() = 0;
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
    std::string playAnimations(
        std::atomic<bool>&,
        const std::vector<Animation>&
    ) override;

    void setTime(int, int) override;
    void setSentence(std::string) override;
    void clearBoard() override;
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


// How to use
//Hardware& hw = Hardware::instance();
//hw.bluetooth_init();
//or
//hardware::instance().clearLed();

