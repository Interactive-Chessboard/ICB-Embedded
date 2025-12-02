// lights.cpp
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include "board.hpp"


void setLed(std::array<LedColor, 64>)
{

}


void clearLed(void)
{
    std::array<LedColor, 64> leds;
    return setLed(leds);
}


std::string playAnimations(std::atomic<bool>& stop, const std::vector<Animation>& animations)
{
    for (auto& animation : animations)
    {
        setLed(animation.leds);
        for (int i = 0; i < animation.display_time_ms / 10; i++)
        {
            if (stop.load())
            {
                clearLed();
                return "Error, stopped";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }   
    }
    clearLed();
    return "ok";
}