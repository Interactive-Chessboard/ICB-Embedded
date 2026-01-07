// lights.cpp
#include "board.hpp"

std::array<LedColor, 64> current_global_light;

void Board::setLed(std::array<LedColor, 64> leds)
{
    if (leds == current_global_light)
        return;

    // Display lights on hardware




    current_global_light = leds;
}


void Board::clearLed(void)
{
    std::array<LedColor, 64> leds;
    return setLed(leds);
}


std::string Board::playAnimations(std::atomic<bool>& stop, const std::vector<Animation>& animations)
{
    for (auto& animation : animations)
    {
        Board::setLed(animation.leds);
        for (int _ = 0; _ < animation.display_time_ms / 10; _++)
        {
            if (stop.load())
            {
                Board::clearLed();
                return "Error, stopped";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    Board::clearLed();
    return "ok";
}
