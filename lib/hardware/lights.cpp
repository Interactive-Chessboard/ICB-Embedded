// lights.cpp
#include "real_hardware.hpp"
#ifdef ARDUINO


void RealHardware::setLed(const std::array<LedColor, 64>& leds)
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

#endif
