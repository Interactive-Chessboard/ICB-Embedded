// opponent.hpp
#pragma once
#include "move_struct.hpp"
#include "clock_struct.hpp"
namespace Opponent
{
Move getOnlineMove(ClockSetting&);
Move getBotMove(ClockSetting&);
};