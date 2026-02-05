// hardware_interface.hpp
#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <array>


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
private:
    virtual void handleOtaWrite(const std::string&) = 0;
public:
    virtual ~IHardware() = default;

    // Hardware
    virtual std::string getFirmwareVersion() = 0;
    virtual std::string getMacAddress() = 0;

    // bluetooth
    virtual void bluetoothInit() = 0;
    virtual std::string getBluetoothMessage() = 0;
    virtual void sendBluetoothMessage(const std::string&) = 0;

    // board
    virtual uint64_t getBoardArr() = 0;

    // lights
    virtual void setLed(const std::array<LedColor, 64>&) = 0;
    virtual void clearLed() = 0;

    // screen
    virtual void reserveScreen(bool) = 0;
    virtual void setTimeScreen(const std::vector<std::string>&) = 0;
    virtual void setScreen(const std::vector<std::string>&, int) = 0;
    virtual void clearScreen() = 0;

    // buttons
    virtual bool detectSelectClick() = 0;
    virtual bool detectStartClick() = 0;
};
