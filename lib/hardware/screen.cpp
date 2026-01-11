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


void RealHardware::setOptions(std::vector<std::string> options, int selected)
{
    if (selected >= options.size())
        return;
}


void RealHardware::clearScreen()
{

}
