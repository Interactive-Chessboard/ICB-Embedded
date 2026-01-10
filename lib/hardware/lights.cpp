// lights.cpp
#include "hardware.hpp"

std::array<LedColor, 64> current_global_light;

void RealHardware::setLed(std::array<LedColor, 64> leds)
{
    if (leds == current_global_light)
        return;

    // Display lights on hardware




    current_global_light = leds;
}


void RealHardware::clearLed(void)
{
    std::array<LedColor, 64> leds;
    return setLed(leds);
}


std::string RealHardware::playAnimations(std::atomic<bool>& stop, const std::vector<Animation>& animations)
{
    for (auto& animation : animations)
    {
        RealHardware::setLed(animation.leds);
        for (int _ = 0; _ < animation.display_time_ms / 10; _++)
        {
            if (stop.load())
            {
                RealHardware::clearLed();
                return "Error, stopped";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    RealHardware::clearLed();
    return "ok";
}
