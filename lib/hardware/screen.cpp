// screen.cpp
#include "real_hardware.hpp"
#ifdef ARDUINO


void RealHardware::reserveScreen(bool reserve)
{
    screen_reserved = reserve;
}


void RealHardware::clearScreen()
{
    std::lock_guard<std::mutex> lock(screen_mutex);

    if (screen_reserved)
        return;

    // Clear screen

}


void RealHardware::setTimeScreen(const std::vector<std::string>& text)
{
    std::lock_guard<std::mutex> lock(screen_mutex);

    if (screen_reserved)
        return;

    // Display the times here
}


void RealHardware::setScreen(const std::vector<std::string>& text, int selected)
{
    std::lock_guard<std::mutex> lock(screen_mutex);

    if (selected < 0 || selected >= text.size())
        return;

    // Selected mean having '>' before the text
}

#endif
