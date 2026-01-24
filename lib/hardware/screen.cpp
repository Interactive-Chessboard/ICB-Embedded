// screen.cpp
#include "hardware.hpp"


void RealHardware::reserveScreen(bool reserve)
{
    screen_reserved = reserve;
}


void RealHardware::setTimeScreen(std::vector<std::string> text)
{
    std::lock_guard<std::mutex> lock(screen_mutex);

    if (screen_reserved)
        return;

    // Display the times here
}


void RealHardware::setScreen(std::vector<std::string> text, int selected)
{
    std::lock_guard<std::mutex> lock(screen_mutex);

    if (selected >= text.size())
        return;

    // Selected mean having '>' before the text
}
