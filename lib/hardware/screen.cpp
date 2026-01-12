// screen.cpp
#include "hardware.hpp"

void RealHardware::setTime(int white_time_ms, int black_time_ms)
{

}


bool RealHardware::detectSelectClick()
{
    return false;
}


bool RealHardware::detectStartClick()
{
    return false;
}


void RealHardware::setScreen(std::vector<std::string> text, int selected)
{
    if (selected >= text.size())
        return;

    // Selected mean having '>' before the text
}


void RealHardware::clearScreen()
{

}
