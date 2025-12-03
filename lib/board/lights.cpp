// lights.cpp
#include "board.hpp"


void Board::setLed(std::array<LedColor, 64>)
{

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