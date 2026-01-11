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

